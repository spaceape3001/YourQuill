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
    std::pair<UAtom,bool>    UAtom::create(Document doc)
    {
        bool    created = false;
        Atom x  = cdb::db_atom(doc, &created);
        return { x, created };
    }

    UAtom  UAtom::get(Atom x)
    {
        return UAtom(x);
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
    
    void    UAtom::execute(const AtomChangeData&acd)
    {
        for(const AtomNotifier* an : AtomNotifier::all()){
            if(!an) [[unlikely]]
                continue;
            if(triggered(acd, *an))
                an -> change(acd);
        }
    }

    void    UAtom::changed(KVTree&attrs, Document doc)
    {
        auto [u,cr] = create(doc);
        if(cr){
            u.i_title(attrs);
            u.i_classes(attrs);
            u.i_tags(attrs);
            u.i_abbr(attrs);
            u.i_attributes(attrs);
            u.i_notify();
        } else {
            AtomChangeData  acd;
            acd.doc             = doc;
            acd.atom            = u.x;
            acd.title           = u.u_title(attrs);
            acd.classes         = u.u_classes(attrs);
            acd.abbreviation    = u.u_abbr(attrs);
            acd.tags            = u.u_tags(attrs);
            u.u_attributes(attrs);
                // clean this up later....
            execute(acd); 
        }
    }
    
    Atom    UAtom::s3_create(KVTree&attrs, Document doc)
    {
        auto [u,cr] = create(doc);
        u.i_title(attrs);
        u.i_classes(attrs);
        u.i_tags(attrs);
        u.i_abbr(attrs);
        u.i_attributes(attrs);
        u.i_notify();
        return u.x;
    }
    
    void    UAtom::s3_bind(Document doc)
    {
    }
    
    void    UAtom::s3_notify(Document doc)
    {
        auto [u,cr] = create(doc);
        u.i_notify();
    }
    
    

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    UAtom::UAtom(Atom _x) : x(_x), id(_x.id)
    {
    }

    void    UAtom::u_icon(Image img)
    {
        static thread_local CacheQuery uIcon("UPDATE Atoms SET icon=? WHERE id=?");
        uIcon.exec(img.id, id);
    }

    void    UAtom::i_atom(KVTree&attrs)
    {
        
    }
    
    void    UAtom::u_atom(KVTree& attrs)
    {
    }

    void    UAtom::i_notify()
    {
        AtomChangeData  acd;
        acd.atom            = x;
        acd.doc             = cdb::document(x);
        acd.classes.added   = make_set(cdb::classes(x));
        acd.tags.added      = make_set(cdb::tags(x));
        acd.title           = true;
        acd.abbreviation    = true;
        execute(acd);

        for(Atom a : cdb::children(x)){
            auto u = get(a);
            u.i_notify();
        }
    }


    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ABBREVIATION

    std::string         UAtom::q_abbr(const KVTree& attrs)
    {
        return std::string(attrs.value(svL_aAbbreviation));
    }
    
    void                UAtom::i_abbr(const KVTree& attrs)
    {
        static thread_local CacheQuery uTitle("UPDATE Atoms SET abbr=? WHERE id=?");
        std::string    t   = q_abbr(attrs);
        uTitle.bind(1, t);
        uTitle.bind(2, id);
        uTitle.exec();
    }
    
    bool                UAtom::u_abbr(const KVTree& attrs)
    {
        std::string     n   = cdb::abbreviation(x);
        static thread_local CacheQuery uTitle("UPDATE Atoms SET abbr=? WHERE id=?");
        std::string    t   = q_abbr(attrs);
        if(t != n){
            uTitle.bind(1, t);
            uTitle.bind(2, id);
            uTitle.exec();
            return true;
        } else
            return false;
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  CLASSES

    void                UAtom::i_classes(const ClassSet& classes)
    {
        static thread_local CacheQuery  iClass("INSERT INTO AClasses (atom, class, hops) VALUES (?,?,?)");
        for(auto& itr : cdb::base_classes_ranked_merged_map(classes, true)){
            iClass.bind(1, id);
            iClass.bind(2, itr.first.id);
            iClass.bind(3, itr.second.cnt);
            iClass.exec();
        }
    }
    
    ClassSet            UAtom::i_classes(KVTree&attrs, bool autoDeleteKeys)
    {
        auto clsset     = make_set(cdb::db_classes(attrs.values_set(svL_aClass)));
        if(autoDeleteKeys)
            attrs.erase_all(svL_aClass);
        i_classes(clsset);
        return clsset;
    }

    SetChanges<Class>   UAtom::u_classes(const ClassSet& classes)
    {
        SetChanges<Class>   ret;
        ClassCountMap   omap    = cdb::classes_and_hops_map(x);
        ClassCountMap   nmap    = cdb::base_classes_ranked_merged_map(classes, true);
        map_difference_exec(omap, nmap, 
            [&](const ClassCountMap::value_type&pp){    // REMOVED
                ret.removed.insert(pp.first);

                static thread_local CacheQuery  d("DELETE FROM AClasses WHERE atom=? AND class=?");
                auto d_af   = d.af();
                d.bind(1, id);
                d.bind(2, pp.first.id);
                d.exec();
            },
            [&](const ClassCountMap::value_type& pp){   // MODIFIED
                ret.modified.insert(pp.first);

                static thread_local CacheQuery  u("UPDATE AClasses SET hops=? WHERE atom=? AND class=?");
                auto u_af   = u.af();
                u.bind(1, pp.second);
                u.bind(2, id);
                u.bind(3, pp.first.id);
                u.exec();
            },
            [&](const ClassCountMap::value_type& pp){   // INSERTED
                ret.added.insert(pp.first);
                
                static thread_local CacheQuery  i("INSERT INTO AClasses (atom, class, hops) VALUES (?,?,?)");
                auto i_af   = i.af();
                i.bind(1, id);
                i.bind(2, pp.first.id);
                i.bind(3, pp.second);
                i.exec();
            }
        );
        return ret;
    }

    SetChanges<Class>   UAtom::u_classes(KVTree& attrs, bool autoDeleteKeys)
    {
        auto clsset     = make_set(cdb::db_classes(attrs.values_set(svL_aClass)));
        if(autoDeleteKeys)
            attrs.erase_all(svL_aClass);
        return u_classes(clsset);
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  PROPERTIES

    Atom::Property  UAtom::i_prop(Attribute attr)
    {
        Atom::Property ap = cdb::db_atom_property(x, attr, Field());
        
        //  TODO....
        
        return ap;
    }

    void    UAtom::i_props(std::span<const Attribute::Diff> rep)
    {
        for(const Attribute::Diff& i : rep){
            if(i.added() && i.attr)
                i_prop(i.attr);
        }
    }

    void    UAtom::u_props(std::span<const Attribute::Diff>rep)
    {
        for(const Attribute::Diff& i : rep){
            if(i.added() && i.attr)
                i_prop(i.attr);
            //if(i.deleted() && i.attr)
                //x_prop(i.attr);
        }
    }

/*
    void    UAtom::x_prop(Attribute attr)
    {   
        static thread_local CacheQuery  xProp("DELETE FROM AProperties WHERE atom=? AND attr=?");
        xProp.bind(1, id);
        xProp.bind(2, attr.id);
        xProp.exec();
    }
*/

    void    UAtom::x_props(std::span<const Attribute::Diff> rep)
    {
    /*
        for(const Attribute::Diff& i : rep){
            if(i.deleted() && i.attr)
                x_prop(i.attr);
        }
    */
    }

    void    UAtom::i_attributes(const KVTree&attrs)
    {
        auto rep    = diff::additions(cdb::document(x), attrs);
        rep.exec_inserts();
        i_props(rep.items);
    }
    
    void    UAtom::u_attributes(const KVTree&attrs)
    {
        auto rep    = diff::changes(cdb::document(x), attrs);
        rep.exec_inserts();
        u_props(rep.items);
        rep.exec_reindex();
        x_props(rep.items);
        rep.exec_removals();
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  TAGS

    void                UAtom::i_tags(const TagSet& tags)
    {
        static thread_local CacheQuery iTag("INSERT INTO ATags (atom, tag) VALUES (?,?)");
        for(Tag t : tags){
            iTag.bind(1, id);
            iTag.bind(2, t.id);
            iTag.exec();
        }
    }
    
    TagSet              UAtom::i_tags(KVTree&attrs, bool autoDeleteKeys)
    {
        auto tagset = make_set(cdb::db_tags(attrs.values_set(svL_aTag)));
        if(autoDeleteKeys)
            attrs.erase_all(svL_aTag);
        i_tags(tagset);
        return tagset;
    }
    
    SetChanges<Tag>     UAtom::u_tags(const TagSet& tags)
    {
        TagSet              otags   = make_set(cdb::tags(x));
        SetChanges<Tag> ret = add_remove(otags, tags);
        if(!ret.removed.empty()){
            static thread_local CacheQuery dTag("DELETE FROM ATags WHERE atom=? AND tag=?");
            for(Tag t : ret.removed)
                dTag.exec(id, t.id);
        }
        if(!ret.added.empty()){
            static thread_local CacheQuery iTag("INSERT INTO ATags (atom, tag) VALUES (?, ?)");
            for(Tag t : ret.added)
                iTag.exec(id, t.id);
        }
        return ret;
    }
    
    SetChanges<Tag>     UAtom::u_tags(KVTree&attrs, bool autoDeleteKeys)
    {
        auto tagset = make_set(cdb::db_tags(attrs.values_set(svL_aTag)));
        if(autoDeleteKeys)
            attrs.erase_all(svL_aTag);
        return u_tags(tagset);
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  TITLE

    std::string         UAtom::q_title(const KVTree& attrs) const
    {
        std::string    t   = std::string(attrs.value(svL_aTitle));
        if(!t.empty())
            return t;
        return cdb::key(x);
    }
    
    void                UAtom::i_title(const KVTree& attrs)
    {
        static thread_local CacheQuery uTitle("UPDATE Atoms SET name=? WHERE id=?");
        std::string    t   = q_title(attrs);
        uTitle.bind(1, t);
        uTitle.bind(2, id);
        uTitle.exec();
    }
    
    bool                UAtom::u_title(const KVTree& attrs)
    {
        std::string     n   = cdb::name(x);
        static thread_local CacheQuery uTitle("UPDATE Atoms SET name=? WHERE id=?");
        std::string    t   = q_title(attrs);
        if(t != n){
            uTitle.bind(1, t);
            uTitle.bind(2, id);
            uTitle.exec();
            return true;
        } else
            return false;
    }
}

