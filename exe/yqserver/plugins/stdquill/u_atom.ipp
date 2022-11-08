////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "u_atom.hpp"
#include <basic/Vector.hpp>
#include <kernel/atom/AtomCDB.hpp>
#include <kernel/attr/AttributeCDB.hpp>
#include <kernel/db/SQ.hpp>
#include <kernel/io/Strings.hpp>

namespace {

    /*
    
        On creation
        
            -> classes insert (immedate)
            -> attributes insert
            -> property conversion (w/o edge detection)
            
        
            -> class insert notifications
            
            -> field insert notifications
            
            
            Note, we're going to defer inferrence & change considerations for later... :) 
    
    */

    
    using ClsHopsMap   = std::map<Class,uint64_t>;

    //! Used for inserting classes on a (known to be) new atom
    //! \param[in]  Atom to associate these classes with
    //! \param[in]  
    void    ati_classes(Atom at, const ClassSet& classes)
    {
        static thread_local SQ  iClass("INSERT INTO AClasses (atom, class, hops) VALUES (?,?,?)");
        for(auto& itr : cdb::base_classes_ranked_merged_map(classes, true)){
            iClass.bind(1, at.id);
            iClass.bind(2, itr.first.id);
            iClass.bind(3, itr.second.cnt);
            iClass.exec();
        }
    }

    //! Used for inserting classes on a (known to be) new atom
    void    ati_classes(Atom at, const string_view_set_t& clskeys)
    {
        ati_classes(at, make_set(cdb::db_classes(clskeys)));
    }
    
    void    ati_classes(Atom at, const KVTree& attrs)
    {
        ati_classes(at, attrs.values_set(svL_aClass));
    }
    
    SetChanges<Class>   atu_classes(Atom at, const ClassSet& classes)
    {
        SetChanges<Class>   ret;
        ClassCountMap   omap    = cdb::classes_and_hops_map(at);
        ClassCountMap   nmap    = cdb::base_classes_ranked_merged_map(classes, true);
        map_difference_exec(omap, nmap, 
            [&](const ClassCountMap::value_type&pp){    // REMOVED
                ret.removed.insert(pp.first);

                static thread_local SQ  d("DELETE FROM AClasses WHERE atom=? AND class=?");
                auto d_af   = d.af();
                d.bind(1, at.id);
                d.bind(2, pp.first.id);
                d.exec();
            },
            [&](const ClassCountMap::value_type& pp){   // MODIFIED
                ret.modified.insert(pp.first);

                static thread_local SQ  u("UPDATE AClasses SET hops=? WHERE atom=? AND class=?");
                auto u_af   = u.af();
                u.bind(1, pp.second);
                u.bind(2, at.id);
                u.bind(3, pp.first.id);
                u.exec();
            },
            [&](const ClassCountMap::value_type& pp){   // INSERTED
                ret.added.insert(pp.first);
                
                static thread_local SQ  i("INSERT INTO AClasses (atom, class, hops) VALUES (?,?,?,?)");
                auto i_af   = i.af();
                i.bind(1, at.id);
                i.bind(2, pp.first.id);
                i.bind(3, pp.second);
                i.exec();
            }
        );
        return ret;
    }

    SetChanges<Class>   atu_classes(Atom at, const string_view_set_t& clskeys)
    {
        return atu_classes(at, make_set(cdb::db_classes(clskeys)));
    }
    
    SetChanges<Class>   atu_classes(Atom at, const KVTree& attrs)
    {
        return atu_classes(at, attrs.values_set(svL_aClass));
    }

    //  ---------------------------------------------------------------------------------------------------------------

    void    ati_tags(Atom at, const TagSet& tags)
    {
        static thread_local SQ iTag("INSERT INTO ATags (atom, tag) VALUES (?,?)");
        for(Tag t : tags){
            iTag.bind(1, at.id);
            iTag.bind(2, t.id);
            iTag.exec();
        }
    }

    void    ati_tags(Atom at, const string_view_set_t& tagkeys)
    {
        ati_tags(at, make_set(cdb::db_tags(tagkeys)));
    }
    
    void    ati_tags(Atom at, const KVTree& attrs)
    {
        ati_tags(at, attrs.values_set(svL_aTag));
    }
    
    SetChanges<Tag>    atu_tags(Atom at, const TagSet& tags)
    {
        TagSet              otags   = make_set(cdb::tags(at));
        SetChanges<Tag> ret = add_remove(otags, tags);
        if(!ret.removed.empty()){
            static thread_local SQ dTag("DELETE FROM ATags WHERE atom=? AND tag=?");
            for(Tag t : ret.removed)
                dTag.exec(at.id, t.id);
        }
        if(!ret.added.empty()){
            static thread_local SQ iTag("INSERT INTO ATags (atom, tag) VALUES (?, ?)");
            for(Tag t : ret.added)
                iTag.exec(at.id, t.id);
        }
        return ret;
    }
    
    SetChanges<Tag>    atu_tags(Atom at, const string_view_set_t& tagkeys)
    {
        return atu_tags(at, make_set(cdb::db_tags(tagkeys)));
    }

    SetChanges<Tag>    atu_tags(AtomChangeData& acd, Atom at, const KVTree& attrs)
    {
        return atu_tags(at, attrs.values_set(svL_aTag));
    }

    //  ---------------------------------------------------------------------------------------------------------------
    
    std::string     atq_title(Atom at, const KVTree& attrs)
    {
        std::string    t   = std::string(attrs.value(svL_aTitle));
        if(!t.empty())
            return t;
        return cdb::key(at);
    }
    
    void    ati_title(Atom at, const KVTree& attrs)
    {
        static thread_local SQ uTitle("UPDATE Atoms SET name=? WHERE id=?");
        std::string    t   = atq_title(at, attrs);
        uTitle.bind(1, t);
        uTitle.bind(2, at);
        uTitle.exec();
    }
    
    bool    atu_title(Atom at, const KVTree& attrs)
    {
        std::string     n   = cdb::name(at);
        static thread_local SQ uTitle("UPDATE Atoms SET name=? WHERE id=?");
        std::string    t   = atq_title(at, attrs);
        if(t != n){
            uTitle.bind(1, t);
            uTitle.bind(2, at);
            uTitle.exec();
            return true;
        } else
            return false;
    }

    //  ---------------------------------------------------------------------------------------------------------------
    
    void    ati_attributes(Atom at, const KVTree& attrs)
    {
        Document    doc = cdb::document(at);
        auto rep    = diff::additions(doc, attrs);
        rep.exec_inserts();
    }

    void    atu_attributes(Atom at, const KVTree& attrs)
    {
        Document    doc = cdb::document(at);
        auto rep = diff::changes(doc, attrs);
        rep.exec_inserts();
        rep.exec_reindex();
        rep.exec_removals();
    }

    //  ---------------------------------------------------------------------------------------------------------------

    void    u_atom_execute(const AtomChangeData&)
    {
        // TODO.....
    }

    void    i_atom_notify(Atom x, bool recursive=true)
    {
        AtomChangeData  acd;
        acd.atom            = x;
        acd.doc             = cdb::document(x);
        acd.classes.added   = make_set(cdb::classes(x));
        acd.tags.added      = make_set(cdb::tags(x));
        acd.title           = true;
        u_atom_execute(acd);
    }


    Atom    s3_atom_create(const KVTree& attrs, Document doc)
    {
        Atom        x   = cdb::db_atom(doc);
        ati_classes(x, attrs);
        ati_title(x, attrs);
        ati_tags(x, attrs);
        ati_attributes(x, attrs);
        return x;
    }
    
    void    s3_atom_bind(Atom, bool recursive=true)
    {
    }

    void    s3_atom_notify(Atom x, bool recursive=true)
    {
        i_atom_notify(x, recursive);
    }
    
    void    i_atom(Atom x, const KVTree& attrs)
    {
        ati_classes(x, attrs);
        ati_title(x, attrs);
        ati_tags(x, attrs);
        ati_attributes(x, attrs);
        i_atom_notify(x);
    }
    
    void    u_atom(Atom x, const KVTree& attrs)
    {
        AtomChangeData acd;
        acd.doc         = cdb::document(x);
        acd.atom        = x;
        acd.classes     = atu_classes(x, attrs);
        acd.title       = atu_title(x, attrs);
        acd.tags        = atu_tags(acd, x, attrs);
        atu_attributes(x, attrs);
        u_atom_execute(acd);
    }
    
    Atom    u_atom(const KVTree& attrs, Document doc)
    {
        bool    created = false;
        Atom    x   = cdb::db_atom(doc, &created);
        if(created){
            i_atom(x, attrs);
        } else {
            u_atom(x, attrs);
        }
        return x;
    }
    

#if 0
    

    void    u_atom(Atom at, Document doc, const Attribute::Report& rep, cdb_options_t opts, const ClassSet& cset)
    {
//yInfo() << "Update on " << cdb::key(at) << " ... with " << cset.size() << " class(es)";
        ClassSet    oldcls    = make_set(cdb::classes(at));
        ClsHopsMap  newhops;
        //determine_hops(newhops, cset);
        //update_aclasses(at, doc, newhops);
        ClassSet    newcls    = make_set(cdb::classes(at));
        auto        clschgs   = changes(oldcls, newcls);
        
        static auto&    anotes  = AtomNotifier::change_map();
        
        for(Class c : clschgs.removed){
            for(const AtomNotifier* an : anotes[Change::Removed]){
                if(!an)
                    continue;
                //an -> change(at, c, Change::Removed);
            }
        }
        
        for(Class c : clschgs.added){
            for(const AtomNotifier* an : anotes[Change::Added]){
                if(!an)
                    continue;
                //an -> change(at, c, Change::Added);
            }
        }
        
        for(Class c : clschgs.same){
            for(const AtomNotifier* an : anotes[Change::Modified]){
                if(!an)
                    continue;
                //an -> change(at, c, Change::Modified);
            }
        }
    }
#endif

}

