////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "uAtom.hpp"
#include "uClass.hpp"
#include "uField.hpp"

#include <basic/CollectionUtils.hpp>
#include <basic/Logging.hpp>
#include <basic/TextUtils.hpp>

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

    void     UAtom::i_atom(Atom x, KVTree& attrs, Class c, bool notify)
    {
        UAtom   u(x);
        
        u.i_title(attrs);
        u.i_abbr(attrs);
        u.i_tags(attrs);
        u.i_classes(attrs, c);

        auto rep    = diff::additions(cdb::document(x), attrs);
        rep.exec_inserts();
        
        u.i_props(rep.items);
        
        if(notify)
            i_notify(x, true);
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
    

    
    void     UAtom::u_atom(Atom x, KVTree& attrs, Class c)
    {
        AtomChangeData  acd;
        
        UAtom       u(x);
        
        acd.doc             = cdb::document(x);
        acd.atom            = x;
        acd.title           = u.u_title(attrs);
        acd.abbreviation    = u.u_abbr(attrs);
        acd.classes         = u.u_classes(attrs);
        acd.tags            = u.u_tags(attrs);

        auto rep    = diff::changes(acd.doc, attrs);
        rep.exec_inserts();
        u.i_props(rep.items);
        u.u_props(rep.items);
        rep.exec_reindex();
        
        //  temp disable
        //u.x_props(rep.items);
        //rep.exec_removals();
        
        acd.diffs       = rep.items;
        
        execute(acd);
    }


    void     UAtom::u_icon(Atom x, Image img)
    {
        static thread_local CacheQuery uIcon("UPDATE Atoms SET icon=? WHERE id=?");
        uIcon.exec(img.id, x.id);
    }
    

    void    UAtom::x_atom(Atom x)
    {
        
    }


    ////////////////////////////////////////////////////////////////////////////

    UAtom::UAtom(Atom x) : m_atom(x), m_id(x.id)
    {
    }


    void    UAtom::i_abbr(KVTree& attrs)
    {
        static thread_local CacheQuery sql("UPDATE Atoms SET abbr=? WHERE id=?");
        std::string    t   = q_abbr(attrs);
        sql.bind(1, t);
        sql.bind(2, m_id);
        sql.exec();
    }

    void     UAtom::i_classes(const ClassSet& classes)
    {
        static thread_local CacheQuery  iClass("INSERT INTO AClasses (atom, class, hops) VALUES (?,?,?)");
        for(auto& itr : cdb::base_classes_ranked_merged_map(classes, true)){
            iClass.bind(1, m_id);
            iClass.bind(2, itr.first.id);
            iClass.bind(3, itr.second.cnt);
            iClass.exec();
            m_classes.insert(itr.first);
        }
    }


    void    UAtom::i_classes(KVTree&attrs, Class c)
    {
        auto clsset     = make_set(cdb::db_classes(attrs.values_set(svL_aClass)));
        if(c)
            clsset.insert(c);
        attrs.erase_all(svL_aClass);
        i_classes(clsset);
    }


    void  UAtom::i_prop(Attribute::Diff& i)
    {
        u_resolve();

        
        auto ap = cdb::db_atom_property(m_atom, i.attr);
        static thread_local CacheQuery uChild("UPDATE AProperties SET ck=?, child=?, class=? WHERE id=?");
        static thread_local CacheQuery uField("UPDATE AProperties SET field=? WHERE id=?");
        static thread_local CacheQuery uTarget("UPDATE AProperties SET target=? WHERE id=?");
        //static thread_local CacheQuery uSource("UPDATE AProperties SET source=? WHERE id=?");

        std::string ck;
        auto    mkChild     = [&](Class c) {
            ck              = i.key;
            size_t      cnt = cdb::count_properties(m_atom, i.key);
            if(cnt > 1)
                ck += to_string(cnt);

            Atom    a   = cdb::db_atom(m_atom, ck);
            
            auto af = uChild.af();
            uChild.bind(1, ck);
            uChild.bind(2, a.id);
            uChild.bind(3, c.id);
            uChild.bind(4, ap.id);
            uChild.exec();
            
            UClass& uc  = UClass::get(c);
            UAtom   ua(a);

            return a;
        };

        auto j  = m_resolve.find(i.key);
        if(j != m_resolve.end()){
            if(auto p = std::get_if<Node>(&j->second)){
                Node    n   = *p;
                Atom a = mkChild(n.cls);
                
                
                
            }
            if(auto p = std::get_if<Outbound>(&j->second)){
                Outbound out = *p;
                Atom a = mkChild(out.cls);

            }
            if(auto p = std::get_if<Field>(&j->second)){
                Field   f   = *p;
                uField.exec(f, ap);
            }
        }
    }


    void    UAtom::i_props(std::span<Attribute::Diff> rep)
    {
        for(Attribute::Diff& i : rep){
            if(i.added() && i.attr)
                i_prop(i);
        }
    }
    
    void     UAtom::i_tags(const TagSet& tags)
    {
        static thread_local CacheQuery iTag("INSERT INTO ATags (atom, tag) VALUES (?,?)");
        for(Tag t : tags){
            iTag.bind(1, m_id);
            iTag.bind(2, t.id);
            iTag.exec();
        }
    }

    void   UAtom::i_tags(KVTree&attrs)
    {
        auto tagset = make_set(cdb::db_tags(attrs.values_set(svL_aTag)));
        attrs.erase_all(svL_aTag);
        i_tags(tagset);
    }

    void    UAtom::i_title(KVTree& attrs)
    {
        static thread_local CacheQuery sql("UPDATE Atoms SET name=? WHERE id=?");
        std::string    t   = q_title(attrs);
        sql.bind(1, t);
        sql.bind(2, m_id);
        sql.exec();
    }

    std::string      UAtom::q_abbr(KVTree&attrs) const
    {
        return std::string(attrs.value(svL_aAbbreviation));
    }

    std::string      UAtom::q_title(KVTree&attrs) const
    {
        std::string    t   = std::string(attrs.value(svL_aTitle));
        if(!t.empty())
            return t;
        return cdb::key(m_atom);
    }

    bool                UAtom::u_abbr(KVTree& attrs)
    {
        std::string     n   = cdb::abbreviation(m_atom);
        static thread_local CacheQuery uTitle("UPDATE Atoms SET abbr=? WHERE id=?");
        std::string    t   = q_abbr(attrs);
        if(t != n){
            uTitle.bind(1, t);
            uTitle.bind(2, m_id);
            uTitle.exec();
            return true;
        } else
            return false;
    }

    SetChanges<Class>   UAtom::u_classes(const ClassSet& classes)
    {
        SetChanges<Class>   ret;
        ClassCountMap   omap    = cdb::classes_and_hops_map(m_atom);
        ClassCountMap   nmap    = cdb::base_classes_ranked_merged_map(classes, true);
        for(auto& i : nmap)
            m_classes.insert(i.first);
        
        map_difference_exec(omap, nmap, 
            [&](const ClassCountMap::value_type&pp){    // REMOVED
                ret.removed.insert(pp.first);

                static thread_local CacheQuery  d("DELETE FROM AClasses WHERE atom=? AND class=?");
                auto d_af   = d.af();
                d.bind(1, m_id);
                d.bind(2, pp.first.id);
                d.exec();
            },
            [&](const ClassCountMap::value_type& pp){   // MODIFIED
                ret.modified.insert(pp.first);

                static thread_local CacheQuery  u("UPDATE AClasses SET hops=? WHERE atom=? AND class=?");
                auto u_af   = u.af();
                u.bind(1, pp.second);
                u.bind(2, m_id);
                u.bind(3, pp.first.id);
                u.exec();
            },
            [&](const ClassCountMap::value_type& pp){   // INSERTED
                ret.added.insert(pp.first);
                
                static thread_local CacheQuery  i("INSERT INTO AClasses (atom, class, hops) VALUES (?,?,?)");
                auto i_af   = i.af();
                i.bind(1, m_id);
                i.bind(2, pp.first.id);
                i.bind(3, pp.second);
                i.exec();
            }
        );

        return ret;
    }

    SetChanges<Class>   UAtom::u_classes(KVTree& attrs, Class c)
    {
        auto clsset     = make_set(cdb::db_classes(attrs.values_set(svL_aClass)));
        if(c)
            clsset.insert(c);
        attrs.erase_all(svL_aClass);
        return u_classes(clsset);
    }

    void    UAtom::u_prop(Attribute::Diff&)
    {
    }

    void    UAtom::u_props(std::span<Attribute::Diff> rep)
    {
        for(Attribute::Diff& i : rep){
            if(i.modified())
                u_prop(i);
        }
    }

    void                UAtom::u_resolve()
    {
        if(m_resolved)
            return ;
        for(Class c : m_classes){
            UClass&     u   = UClass::get(c);
            for(auto& r : u.resolve)
                m_resolve[r.first]  = r.second;
        }
        m_resolved  = true;
    }

   
    SetChanges<Tag>     UAtom::u_tags(const TagSet& tags)
    {
        TagSet              otags   = make_set(cdb::tags(m_atom));
        SetChanges<Tag> ret = add_remove(otags, tags);
        if(!ret.removed.empty()){
            static thread_local CacheQuery dTag("DELETE FROM ATags WHERE atom=? AND tag=?");
            for(Tag t : ret.removed)
                dTag.exec(m_id, t.id);
        }
        if(!ret.added.empty()){
            static thread_local CacheQuery iTag("INSERT INTO ATags (atom, tag) VALUES (?, ?)");
            for(Tag t : ret.added)
                iTag.exec(m_id, t.id);
        }
        return ret;
    }
    
    SetChanges<Tag>     UAtom::u_tags(KVTree&attrs)
    {
        auto tagset = make_set(cdb::db_tags(attrs.values_set(svL_aTag)));
        attrs.erase_all(svL_aTag);
        return u_tags(tagset);
    }
    
    bool    UAtom::u_title(KVTree& attrs)
    {
        std::string     n   = cdb::name(m_atom);
        static thread_local CacheQuery sql("UPDATE Atoms SET name=? WHERE id=?");
        std::string    t   = q_title(attrs);
        if(t != n){
            sql.bind(1, t);
            sql.bind(2, m_id);
            sql.exec();
            return true;
        } else
            return false;
    }

    void    UAtom::x_prop(Attribute::Diff& i)
    {   
        static thread_local CacheQuery  xProp("DELETE FROM AProperties WHERE attr=?");
        xProp.exec(i.attr.id);
    }
    
    void    UAtom::x_props(std::span<Attribute::Diff> rep)
    {
        for(Attribute::Diff& i : rep)
            if(i.deleted())
                x_prop(i);
    }
}

