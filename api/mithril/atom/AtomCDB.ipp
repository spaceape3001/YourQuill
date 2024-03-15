////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/AtomCDB.hpp>
#include <mithril/atom/AtomInfo.hpp>
#include <mithril/attribute/AttributeCDB.hpp>

#include <0/basic/TextUtils.hpp>
#include <mithril/class/Class.hpp>
#include <mithril/class/ClassCDB.hpp>
#include <mithril/bit/NKI.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/field/FieldCDB.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/tag/Tag.hpp>
#include <mithril/tag/TagCDB.hpp>
#include <mithril/wksp/CacheQuery.hpp>
#include <0/math/Counter.hpp>

namespace yq::mithril::cdb {
    std::vector<Atom>   all_atoms(Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT id FROM " TBL_ATOMS " ORDER BY k");
        static thread_local CacheQuery qu("SELECT id FROM " TBL_ATOMS "");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Atom>();
    }
    
    std::vector<Atom>   all_atoms(Class cls, Sorted sorted)
    {
                // I think this SQL is right.....
        static thread_local CacheQuery qs("SELECT atom FROM " TBL_ATOM_CLASS " INNER JOIN " TBL_CLASSES " ON " TBL_ATOM_CLASS ".class=" TBL_CLASSES ".id WHERE class=? ORDER BY " TBL_CLASSES ".k");
        static thread_local CacheQuery qu("SELECT atom FROM " TBL_ATOM_CLASS " WHERE class=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Atom>(cls);
    }
    
    std::vector<Atom>   all_atoms(Document doc, Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT id FROM " TBL_ATOMS " WHERE doc=? ORDER BY " TBL_ATOMS ".k");
        static thread_local CacheQuery qu("SELECT id FROM " TBL_ATOMS " WHERE doc=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Atom>(doc);
    }

    std::vector<Atom>   all_atoms(Tag tag, Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT atom FROM " TBL_ATOM_TAG " INNER JOIN " TBL_ATOMS " ON " TBL_ATOM_TAG ".atom=" TBL_ATOMS ".id WHERE tag=? ORDER BY " TBL_ATOMS ".k");
        static thread_local CacheQuery qu("SELECT atom FROM " TBL_ATOM_TAG " WHERE tag=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Atom>(tag);
    }
    
    size_t   count_atoms()
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM " TBL_ATOMS "");
        return s.size();
    }
    
    size_t   count_atoms(Class cls)
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM " TBL_ATOM_CLASS " WHERE class=?");
        return s.size(cls);
    }
    
    size_t   count_atoms(Document doc)
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM " TBL_ATOMS " WHERE doc=?");
        return s.size(doc);
    }

    size_t   count_atoms(Tag tag)
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM " TBL_ATOM_TAG " WHERE tag=?");
        return s.size(tag);
    }
    
    
    //static Atom     create(Document);

    Atom     find_atom(uint64_t i)
    {
        return exists_atom(i) ? Atom{i} : Atom{};
    }
    
    Atom     find_atom(std::string_view k)
    {
        static thread_local CacheQuery s("SELECT id FROM " TBL_ATOMS " WHERE k=? LIMIT 1");
        return s.as<Atom>(k);
    }
    
    Atom     find_atom(Document doc)
    {
        static thread_local CacheQuery s("SELECT id FROM " TBL_ATOMS " WHERE doc=? AND sk=''");
        return s.as<Atom>(doc);
    }
    
    Atom     find_atom(Document doc, std::string_view ck)
    {
        static thread_local CacheQuery s("SELECT id FROM " TBL_ATOMS " WHERE doc=? AND sk=?");
        return s.as<Atom>(doc, ck);
    }

    std::vector<Atom>    named_atoms(std::string_view n, Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT id FROM " TBL_ATOMS " WHERE name=? ORDER BY k");
        static thread_local CacheQuery qu("SELECT id FROM " TBL_ATOMS " WHERE name=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Atom>(n);
    }
    
    // ////////////////////////////////////

    std::string         abbreviation(Atom a) 
    {
        static thread_local CacheQuery s("SELECT abbr FROM " TBL_ATOMS " WHERE id=?");
        return s.str(a);
    }
    
    std::vector<Attribute>  attributes(Atom a, Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT attr FROM " TBL_ATOM_PROPERTY " INNER JOIN " TBL_ATTRIBUTES " ON " TBL_ATOM_PROPERTY ".attr=" TBL_ATTRIBUTES ".id WHERE atom=? ORDER BY " TBL_ATTRIBUTES ".k");
        static thread_local CacheQuery qu("SELECT attr FROM " TBL_ATOM_PROPERTY " WHERE atom=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Attribute>(a);
    }
    
    std::string         brief(Atom a) 
    {
        static thread_local CacheQuery s("SELECT brief FROM " TBL_ATOMS " WHERE id=?");
        return s.str(a);
    }
    
    std::vector<Atom>   children(Atom a, Sorted sorted) 
    {
        static thread_local CacheQuery qs("SELECT id FROM " TBL_ATOMS " WHERE parent=? ORDER BY k");
        static thread_local CacheQuery qu("SELECT id FROM " TBL_ATOMS " WHERE parent=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Atom>(a);
    }
    
    size_t              children_count(Atom a) 
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM " TBL_ATOMS " WHERE parent=?");
        return s.size(a);
    }

    std::vector<Class>  classes(Atom a, Sorted sorted) 
    {
        static thread_local CacheQuery qs("SELECT class FROM " TBL_ATOM_CLASS " INNER JOIN " TBL_CLASSES " ON " TBL_ATOM_CLASS ".class=" TBL_CLASSES ".id WHERE atom=? ORDER BY " TBL_CLASSES ".k");
        static thread_local CacheQuery qu("SELECT class FROM " TBL_ATOM_CLASS " WHERE atom=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Class>(a);
    }

    size_t              classes_count(Atom a) 
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM " TBL_ATOM_CLASS " WHERE atom=?");
        return s.size(a);
    }
    
    Document            document(Atom a) 
    {
        static thread_local CacheQuery s("SELECT doc FROM " TBL_ATOMS " WHERE id=?");
        return s.as<Document>(a);
    }
    
    bool     exists(Atom a)
    {
        return exists_atom(a.id);
    }
    

    bool     exists_atom(uint64_t i)
    {
        if(!i)
            return false;
            
        static thread_local CacheQuery s("SELECT 1 FROM " TBL_ATOMS " WHERE id=?");
        return s.present(i);
    }

    Image               icon(Atom a) 
    {
        static thread_local CacheQuery s("SELECT icon FROM " TBL_ATOMS " WHERE id=?");
        return s.as<Image>(a);
    }

    Atom::Info          info(Atom a) 
    {
        Atom::Info    ret;
        
        static thread_local CacheQuery s("SELECT k, abbr, brief, name, leaf, parent FROM " TBL_ATOMS " WHERE id=?");
        auto s_af = s.af();
        s.bind(1, a);
        if(s.step() == SQResult::Row){
            ret.key         = s.v_string(1);
            ret.abbr        = s.v_string(2);
            ret.brief       = s.v_string(3);
            ret.name        = s.v_string(4);
            ret.brief       = Leaf{s.v_uint64(5)};
            ret.parent      = Atom{s.v_uint64(6)};
        }
        return ret;
    }
    
    bool                is(Atom a, Class cls) 
    {
        static thread_local CacheQuery    s("SELECT 1 FROM " TBL_ATOM_CLASS " WHERE atom=? AND class=?");
        return s.present(a, cls.id);
    }
    
    bool                is(Atom a, Tag tag) 
    {
        static thread_local CacheQuery    s("SELECT 1 FROM " TBL_ATOM_TAG " WHERE atom=? AND tag=?");
        return s.present(a, tag.id);
    }

    bool                is_edge(Atom a) 
    {
        static thread_local CacheQuery s("SELECT is_edge FROM " TBL_ATOMS " WHERE id=?");
        return s.boolean(a);
    }

    bool                is_document(Atom a)
    {
        return skey(a).empty();
    }

    bool                is_leaf(Atom a) 
    {
        return leaf(a) != Leaf{};
    }

    std::string         key(Atom a) 
    {
        static thread_local CacheQuery s("SELECT k FROM " TBL_ATOMS " WHERE id=?");
        return s.str(a);
    }
    
    std::string         label(Atom a) 
    {
        std::string s   = name(a);
        if(s.empty())
            s   = key(a);
        return s;
    
        //  temporary, fix once able to research
        //static thread_local CacheQuery    s("SELECT ifnull(name,k) FROM " TBL_ATOMS " WHERE id=?");
        //return s.str(id);
    }
    
    Leaf                leaf(Atom a) 
    {
        static thread_local CacheQuery s("SELECT leaf FROM " TBL_ATOMS " WHERE id=?");
        return s.as<Leaf>(a);
    }
    
    std::string         name(Atom a) 
    {
        static thread_local CacheQuery s("SELECT name FROM " TBL_ATOMS " WHERE id=?");
        return s.str(a);
    }
    
    NKI            nki(Atom a, bool autoKey) 
    {
        static thread_local CacheQuery    s("SELECT name,icon,k FROM " TBL_ATOMS " WHERE id=?");
        auto s_af = s.af();
        s.bind(1, a);
        if(s.step() == SQResult::Row){
            NKI  ret;
            ret.name    = s.v_string(1);
            ret.icon    = Image{s.v_uint64(2)};
            ret.key     = s.v_string(3);
            if(autoKey && ret.name.empty())
                ret.name    = ret.key;
            return ret;
        }
        return NKI();
    }

    Atom                parent(Atom a) 
    {
        static thread_local CacheQuery s("SELECT parent FROM " TBL_ATOMS " WHERE id=?");
        return s.as<Atom>(a);
    }

    std::vector<Atom::Property> properties(Atom a, Sorted)
    {
        static thread_local CacheQuery qu("SELECT id FROM " TBL_ATOM_PROPERTY " WHERE atom=?");
        CacheQuery& s = qu; // sorted ? qs : qu;
        return s.vec<Atom::Property>(a);
    }

    std::vector<Atom::Property> properties(Atom a, Attribute attr, Sorted sorted)
    {
        static thread_local CacheQuery qu("SELECT id FROM " TBL_ATOM_PROPERTY " WHERE atom=? AND attr=?");
        CacheQuery& s = qu; // sorted ? qs : qu;
        return s.vec<Atom::Property>(a, attr);
    }
    
    std::vector<Atom::Property> properties(Atom a, Field f, Sorted sorted)
    {
        static thread_local CacheQuery qu("SELECT id FROM " TBL_ATOM_PROPERTY " WHERE atom=? AND field=?");
        CacheQuery& s = qu; // sorted ? qs : qu;
        return s.vec<Atom::Property>(a, f);
    }


    size_t              properties_count(Atom a)
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM " TBL_ATOM_PROPERTY " WHERE atom=?");
        return s.size(a);
    }

    size_t                  properties_count(Atom a, Attribute attr)
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM " TBL_ATOM_PROPERTY " WHERE atom=? AND attr=?");
        return s.size(a, attr);
    }
    
    size_t                  properties_count(Atom a, Field f)
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM " TBL_ATOM_PROPERTY " WHERE atom=? AND field=?");
        return s.size(a, f);
    }
    
    std::string         skey(Atom a) 
    {
        static thread_local CacheQuery s("SELECT sk FROM " TBL_ATOMS " WHERE id=?");
        return s.str(a);
    }
    
    std::vector<Tag>    tags(Atom a, Sorted sorted) 
    {
        static thread_local CacheQuery qs("SELECT tag FROM " TBL_ATOM_TAG " INNER JOIN " TBL_TAGS " ON " TBL_ATOM_TAG ".tag=" TBL_TAGS ".id WHERE atom=? ORDER BY " TBL_TAGS ".k");
        static thread_local CacheQuery qu("SELECT tag FROM " TBL_ATOM_TAG " WHERE atom=?");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Tag>(a);
    }

    size_t              tags_count(Atom a) 
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM " TBL_ATOM_TAG " WHERE atom=?");
        return s.size(a);
    }
    
    std::string         title(Atom a)
    {
        return label(a);
    }
    
    bool                valid(Atom a) 
    {
        return exists_atom(a);
    }

    // ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  Atom Properties
    
    Atom                    atom(Atom::Property p)
    {
        static thread_local CacheQuery s("SELECT atom FROM " TBL_ATOM_PROPERTY " WHERE id=?");
        return s.as<Atom>(p);
    }

    Atom::Property          atom_property(Atom a, Attribute at)
    {
        static thread_local CacheQuery s("SELECT id FROM " TBL_ATOM_PROPERTY " WHERE atom=? AND attr=?");
        auto af = s.af();
        s.bind(1, a.id);
        s.bind(2, at.id);
        if(s.step() == SQResult::Row)
            return Atom::Property(s.v_uint64(1));
        return Atom::Property();
    }
    
    Attribute               attribute(Atom::Property p)
    {
        static thread_local CacheQuery s("SELECT attr FROM " TBL_ATOM_PROPERTY " WHERE id=?");
        return s.as<Attribute>(p);
    }

    Atom                    child(Atom::Property p)
    {
        static thread_local CacheQuery s("SELECT child FROM " TBL_ATOM_PROPERTY " WHERE id=?");
        return s.as<Atom>(p);
    }

    std::string             child_key(Atom::Property p)
    {
        static thread_local CacheQuery s("SELECT ck FROM " TBL_ATOM_PROPERTY " WHERE id=?");
        return s.str(p);
    }

    Class                   class_(Atom::Property p)
    {
        static thread_local CacheQuery s("SELECT class FROM " TBL_ATOM_PROPERTY " WHERE id=?");
        return s.as<Class>(p);
    }

    size_t                  count_properties(Atom a, std::string_view k)
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM " TBL_ATOM_PROPERTY " INNER JOIN " TBL_ATTRIBUTES " ON " TBL_ATOM_PROPERTY ".attr = " TBL_ATTRIBUTES ".id WHERE atom=? AND k=?");
        return s.size(a, k);
    }

    Atom::Property          db_atom_property(Atom a, Attribute at, bool *wasCreated)
    {
        static thread_local CacheQuery s("SELECT id FROM " TBL_ATOM_PROPERTY " WHERE atom=? AND attr=?");
        static thread_local CacheQuery i("INSERT INTO " TBL_ATOM_PROPERTY " (atom, attr) VALUES (?,?)");
        
        auto i_af = i.af();

        i.bind(1, a.id);
        i.bind(2, at.id);
        if(i.exec()){
            if(wasCreated)
                *wasCreated = true;
            return Atom::Property(i.last_id());
        }
        
        if(wasCreated)
            *wasCreated = false;
            
        auto    s_af    = s.af();
        s.bind(1, a.id);
        s.bind(2, at.id);
        
        if(s.step() == SQResult::Row)
            return Atom::Property(s.v_uint64(1));

        yError() << "Unable to get the atom's property from the database: " << key(a) << " " << key(at);
        return Atom::Property();
    }

    bool                    exists_atom_property(uint64_t i)
    {
        if(!i)
            return false;
        static thread_local CacheQuery s("SELECT 1 FROM " TBL_ATOM_PROPERTY " WHERE id=?");
        return s.present(i);
    }
    
    bool                    exists(Atom::Property p)
    {
        return exists_atom_property(p);
    }

    Field                   field(Atom::Property p)
    {
        static thread_local CacheQuery s("SELECT field FROM " TBL_ATOM_PROPERTY " WHERE id=?");
        return s.as<Field>(p);
    }
    
    Atom::PropertyInfo      info(Atom::Property p)
    {
        Atom::PropertyInfo  ret;
        static thread_local CacheQuery s("SELECT atom, attr, field, source, target, child FROM " TBL_ATOM_PROPERTY " WHERE id=?");
        auto s_af = s.af();
        s.bind(1, p.id);
        if(s.step() == SQResult::Row){
            ret.atom    = Atom(s.v_uint64(1));
            ret.attr    = Attribute(s.v_uint64(2));
            ret.field   = Field(s.v_uint64(3));
            ret.source  = Atom(s.v_uint64(4));
            ret.target  = Atom(s.v_uint64(5));
            ret.child   = Atom(s.v_uint64(6));
        }
        return ret;
    }

    std::string             key(Atom::Property p)
    {
        return key(field(p)) + '#' + to_string(p.id);
    }
    
    std::string             name(Atom::Property p)
    {
        return name(field(p));
    }
    
    Atom                    source(Atom::Property p)
    {
        static thread_local CacheQuery s("SELECT source FROM " TBL_ATOM_PROPERTY " WHERE id=?");
        return s.as<Atom>(p);
    }
    
    Atom                    target(Atom::Property p)
    {
        static thread_local CacheQuery s("SELECT target FROM " TBL_ATOM_PROPERTY " WHERE id=?");
        return s.as<Atom>(p);
    }
    

    // ------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------

    
    std::vector<ClassU64Pair> classes_and_hops(Atom a) 
    {
        std::vector<ClassU64Pair> ret;

        static thread_local CacheQuery s("SELECT class, hops FROM " TBL_ATOM_CLASS " WHERE atom=?");
        auto s_af           = s.af();
        
        s.bind(1, a.id);
        
        while(s.step() == SQResult::Row)
            ret.push_back(ClassU64Pair{Class{s.v_uint64(1)}, s.v_uint64(2)});
        
        return ret;
    }

    ClassCountMap               classes_and_hops_map(Atom a)
    {
        ClassCountMap   ret;
        static thread_local CacheQuery s("SELECT class, hops FROM " TBL_ATOM_CLASS " WHERE atom=?");
        auto s_af           = s.af();
        
        s.bind(1, a.id);
        
        while(s.step() == SQResult::Row)
            ret[Class{s.v_uint64(1)}] = { s.v_uint64(2)};
        
        return ret;
    }

    Atom                db_atom(Atom atom, std::string_view ck, bool* wasCreated)
    {
        Document    doc     = cdb::document(atom);
        std::string skey    = Atom::sub_key(atom, ck);
        
        Atom        x       = db_atom(doc, skey, wasCreated);
        if(wasCreated){
            static thread_local CacheQuery  u("UPDATE " TBL_ATOMS " SET parent=? WHERE id=?");
            u.exec(atom, x);
        }
        return x;
    }

    Atom                db_atom(Document d, bool* wasCreated)
    {
        return db_atom(d, std::string(), wasCreated);
    }
    
    Atom                db_atom(Document d, std::string_view ck, bool* wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;


        std::string     dk = Atom::full_key(d, ck);
        if(dk.empty()){
            yError() << "Cannot create to an empty key!";
            return Atom();
        }

        static thread_local CacheQuery    i("INSERT OR FAIL INTO " TBL_ATOMS " (k,doc,sk) VALUES (?,?,?)");
        static thread_local CacheQuery    s("SELECT id FROM " TBL_ATOMS " WHERE k=?");
        auto i_af   = i.af();
        auto s_af   = s.af();
        i.bind(1, dk);
        i.bind(2, d.id);
        i.bind(3, ck);
        
        if(i.exec()){
            if(wasCreated)
                *wasCreated = true;
            return Atom{(uint64_t) i.last_id()};
        } else {
            s.bind(1, dk);
            if(s.step() == SQResult::Row)
                return Atom{s.v_uint64(1)};
            yError() << "Unable to get the atom from the database: " << dk;
            return Atom();
        }
    }
    
    std::vector<Atom>   inbound(Atom a)
    {
        static thread_local CacheQuery s("SELECT edge FROM AEdges WHERE target=?");
        return s.vec<Atom>(a.id);
    }


    std::vector<Atom>   outbound(Atom a)
    {
        static thread_local CacheQuery s("SELECT target FROM AEdges WHERE atom=? AND tgtid!=0");
        return s.vec<Atom>(a.id);
    }
}
