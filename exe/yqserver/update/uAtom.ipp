////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "uAtom.hpp"

#include <basic/Logging.hpp>

#include <mithril/atom/AtomCDB.hpp>
#include <mithril/attribute/AttributeCDB.hpp>
#include <mithril/bit/KeyValue.hpp>
#include <mithril/class/ClassCDB.hpp>
#include <mithril/field/FieldCDB.hpp>
#include <mithril/io/Strings.hpp>
#include <mithril/notify/AtomNotifier.hpp>
#include <mithril/tag/TagCDB.hpp>
#include <mithril/wksp/CacheQuery.hpp>

namespace yq::mithril::update {
    void    UAtom::execute(const AtomChangeData&acd)
    {
        for(const AtomNotifier* an : AtomNotifier::all()){
            if(!an) [[unlikely]]
                continue;
            if(triggered(acd, *an))
                an -> change(acd);
        }
    }

    void    UAtom::i_abbr(Atom x, KVTree& attrs)
    {
        static thread_local CacheQuery sql("UPDATE Atoms SET abbr=? WHERE id=?");
        std::string    t   = q_abbr(x, attrs);
        sql.bind(1, t);
        sql.bind(2, x.id);
        sql.exec();
    }

    void     UAtom::i_atom(Atom x, KVTree&& attrs, Class c, bool notify)
    {
        i_title(x, attrs);
        i_abbr(x, attrs);
        i_tags(x, attrs);
        i_classes(x, attrs, c);

        auto rep    = diff::additions(cdb::document(x), attrs);
        rep.exec_inserts();
        i_props(x, rep.items);
        
        if(notify)
            i_notify(x, true);
    }

    void     UAtom::i_classes(Atom x, const ClassSet& classes)
    {
        static thread_local CacheQuery  iClass("INSERT INTO AClasses (atom, class, hops) VALUES (?,?,?)");
        for(auto& itr : cdb::base_classes_ranked_merged_map(classes, true)){
            iClass.bind(1, x.id);
            iClass.bind(2, itr.first.id);
            iClass.bind(3, itr.second.cnt);
            iClass.exec();
        }
    }

    ClassSet    UAtom::i_classes(Atom x, KVTree&attrs, Class c)
    {
        auto clsset     = make_set(cdb::db_classes(attrs.values_set(svL_aClass)));
        if(c)
            clsset.insert(c);
        attrs.erase_all(svL_aClass);
        i_classes(x, clsset);
        return clsset;
    }


    void     UAtom::i_notify(Atom x, bool recursive)
    {
        AtomChangeData  acd;
        acd.doc             = cdb::document(x);
        acd.atom            = x;
        acd.classes.added   = make_set(cdb::classes(x));
        //acd.fields          = cdb::fields(x);
        acd.tags.added      = make_set(cdb::tags(x));
        acd.title           = true;
        acd.abbreviation    = true;
        execute(acd);
        
        if(recursive){
            for(Atom a : cdb::children(x))
                i_notify(a,true);
        }
        
    }

    Atom::Property  UAtom::i_prop(Atom x, Attribute::Diff& i)
    {
        if(!i.attr)
            return Atom::Property();
            
        Atom::Property ap = cdb::db_atom_property(x, i.attr, Field());
        
        //  TODO....
        
        return ap;
    }

    void    UAtom::i_props(Atom x, std::span<Attribute::Diff> rep)
    {
        for(Attribute::Diff& i : rep){
            if(i.added() && i.attr)
                i_prop(x, i);
        }
    }
    
    void     UAtom::i_tags(Atom x, const TagSet& tags)
    {
        static thread_local CacheQuery iTag("INSERT INTO ATags (atom, tag) VALUES (?,?)");
        for(Tag t : tags){
            iTag.bind(1, x.id);
            iTag.bind(2, t.id);
            iTag.exec();
        }
    }

    TagSet  UAtom::i_tags(Atom x, KVTree&attrs)
    {
        auto tagset = make_set(cdb::db_tags(attrs.values_set(svL_aTag)));
        attrs.erase_all(svL_aTag);
        i_tags(x, tagset);
        return tagset;
    }

    void    UAtom::i_title(Atom x, KVTree& attrs)
    {
        static thread_local CacheQuery sql("UPDATE Atoms SET name=? WHERE id=?");
        std::string    t   = q_title(x, attrs);
        sql.bind(1, t);
        sql.bind(2, x.id);
        sql.exec();
    }

    std::string      UAtom::q_abbr(Atom, KVTree&attrs)
    {
        return std::string(attrs.value(svL_aAbbreviation));
    }

    std::string      UAtom::q_title(Atom x, KVTree&attrs)
    {
        std::string    t   = std::string(attrs.value(svL_aTitle));
        if(!t.empty())
            return t;
        return cdb::key(x);
    }

    
    void     UAtom::s4()
    {
        for(Atom a : cdb::all_atoms()){
            //  And we assimilate edges....
            i_notify(a, false);
        }
    }
    
    bool    UAtom::triggered(const AtomChangeData&acd, const AtomNotifier& an)
    {
        Class   c;
        Field   f;
        const AtomSpec&  as  = an.spec();
        switch(as.type){
        case AtomSpec::Never:
            return false;
        case AtomSpec::Always:
            return true;
        case AtomSpec::ByClassKey:
        case AtomSpec::ByClassId:
            c   = as.class_();
            if(an.change().is_set(Change::Added))
                return acd.classes.added.contains(c);
            if(an.change().is_set(Change::Removed))
                return acd.classes.removed.contains(c);
            if(an.change().is_set(Change::Modified))
                return acd.classes.modified.contains(c);
            if(an.change().is_set(Change::None))
                return acd.classes.same.contains(c);
            return false;
        case AtomSpec::ByFieldKey:
        case AtomSpec::ByFieldId:
            f   = as.field();
            if(an.change().is_set(Change::Added))
                return acd.fields.added.contains(f);
            if(an.change().is_set(Change::Removed))
                return acd.fields.removed.contains(f);
            if(an.change().is_set(Change::Modified))
                return acd.fields.modified.contains(f);
            if(an.change().is_set(Change::None))
                return acd.fields.same.contains(f);
            return false;
        default:
            return false;
        }
    }
    

    bool                UAtom::u_abbr(Atom x, KVTree& attrs)
    {
        std::string     n   = cdb::abbreviation(x);
        static thread_local CacheQuery uTitle("UPDATE Atoms SET abbr=? WHERE id=?");
        std::string    t   = q_abbr(x, attrs);
        if(t != n){
            uTitle.bind(1, t);
            uTitle.bind(2, x.id);
            uTitle.exec();
            return true;
        } else
            return false;
    }

    
    void     UAtom::u_atom(Atom x, KVTree&& attrs, Class c)
    {
        AtomChangeData  acd;
        acd.doc             = cdb::document(x);
        acd.atom            = x;
        acd.title           = u_title(x, attrs);
        acd.abbreviation    = u_abbr(x, attrs);
        acd.classes         = u_classes(x, attrs);
        acd.tags            = u_tags(x, attrs);

        auto rep    = diff::changes(acd.doc, attrs);
        rep.exec_inserts();
        i_props(x, rep.items);
        u_props(x, rep.items);
        rep.exec_reindex();
        x_props(x, rep.items);
        rep.exec_removals();
        
        acd.diffs       = rep.items;
        
        execute(acd);
    }

    SetChanges<Class>   UAtom::u_classes(Atom x, const ClassSet& classes)
    {
        SetChanges<Class>   ret;
        ClassCountMap   omap    = cdb::classes_and_hops_map(x);
        ClassCountMap   nmap    = cdb::base_classes_ranked_merged_map(classes, true);
        map_difference_exec(omap, nmap, 
            [&](const ClassCountMap::value_type&pp){    // REMOVED
                ret.removed.insert(pp.first);

                static thread_local CacheQuery  d("DELETE FROM AClasses WHERE atom=? AND class=?");
                auto d_af   = d.af();
                d.bind(1, x.id);
                d.bind(2, pp.first.id);
                d.exec();
            },
            [&](const ClassCountMap::value_type& pp){   // MODIFIED
                ret.modified.insert(pp.first);

                static thread_local CacheQuery  u("UPDATE AClasses SET hops=? WHERE atom=? AND class=?");
                auto u_af   = u.af();
                u.bind(1, pp.second);
                u.bind(2, x.id);
                u.bind(3, pp.first.id);
                u.exec();
            },
            [&](const ClassCountMap::value_type& pp){   // INSERTED
                ret.added.insert(pp.first);
                
                static thread_local CacheQuery  i("INSERT INTO AClasses (atom, class, hops) VALUES (?,?,?)");
                auto i_af   = i.af();
                i.bind(1, x.id);
                i.bind(2, pp.first.id);
                i.bind(3, pp.second);
                i.exec();
            }
        );
        return ret;
    }

    SetChanges<Class>   UAtom::u_classes(Atom x, KVTree& attrs, Class c)
    {
        auto clsset     = make_set(cdb::db_classes(attrs.values_set(svL_aClass)));
        if(c)
            clsset.insert(c);
        attrs.erase_all(svL_aClass);
        return u_classes(x, clsset);
    }

    void     UAtom::u_icon(Atom x, Image img)
    {
        static thread_local CacheQuery uIcon("UPDATE Atoms SET icon=? WHERE id=?");
        uIcon.exec(img.id, x.id);
    }
    
    void    UAtom::u_prop(Atom x, Attribute::Diff&)
    {
    }

    void    UAtom::u_props(Atom x, std::span<Attribute::Diff> rep)
    {
        for(Attribute::Diff& i : rep){
            if(i.modified())
                u_prop(x, i);
        }
    }

    SetChanges<Tag>     UAtom::u_tags(Atom x, const TagSet& tags)
    {
        TagSet              otags   = make_set(cdb::tags(x));
        SetChanges<Tag> ret = add_remove(otags, tags);
        if(!ret.removed.empty()){
            static thread_local CacheQuery dTag("DELETE FROM ATags WHERE atom=? AND tag=?");
            for(Tag t : ret.removed)
                dTag.exec(x.id, t.id);
        }
        if(!ret.added.empty()){
            static thread_local CacheQuery iTag("INSERT INTO ATags (atom, tag) VALUES (?, ?)");
            for(Tag t : ret.added)
                iTag.exec(x.id, t.id);
        }
        return ret;
    }
    
    SetChanges<Tag>     UAtom::u_tags(Atom x, KVTree&attrs)
    {
        auto tagset = make_set(cdb::db_tags(attrs.values_set(svL_aTag)));
        attrs.erase_all(svL_aTag);
        return u_tags(x, tagset);
    }
    
    bool    UAtom::u_title(Atom x, KVTree& attrs)
    {
        std::string     n   = cdb::name(x);
        static thread_local CacheQuery sql("UPDATE Atoms SET name=? WHERE id=?");
        std::string    t   = q_title(x, attrs);
        if(t != n){
            sql.bind(1, t);
            sql.bind(2, x.id);
            sql.exec();
            return true;
        } else
            return false;
    }

    void    UAtom::x_atom(Atom x)
    {
        
    }

    void    UAtom::x_prop(Atom, Attribute::Diff& i)
    {   
        static thread_local CacheQuery  xProp("DELETE FROM AProperties WHERE attr=?");
        xProp.exec(i.attr.id);
    }
    
    void    UAtom::x_props(Atom x, std::span<Attribute::Diff> rep)
    {
        for(Attribute::Diff& i : rep)
            if(i.deleted())
                x_prop(x,i);
    }
}

