////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AtomCDB.hpp"
#include <basic/TextUtils.hpp>
#include <mithril/kernel/atom/Class.hpp>
#include <mithril/kernel/db/NKI.hpp>
#include <mithril/kernel/file/DocumentCDB.hpp>
#include <mithril/kernel/image/Image.hpp>
#include <mithril/kernel/org/Tag.hpp>
#include <mithril/kernel/wksp/CacheQuery.hpp>
#include <math/Counter.hpp>

namespace yq {
    namespace cdb {
    
        std::vector<Atom>   all_atoms(Sorted sorted)
        {
            static thread_local CacheQuery qs("SELECT id FROM Atoms ORDER BY k");
            static thread_local CacheQuery qu("SELECT id FROM Atoms");
            CacheQuery& s = sorted ? qs : qu;
            return s.vec<Atom>();
        }
        
        std::vector<Atom>   all_atoms(Class cls, Sorted sorted)
        {
                    // I think this SQL is right.....
            static thread_local CacheQuery qs("SELECT atom FROM AClasses INNER JOIN Classes ON AClasses.class=Classes.id WHERE class=? ORDER BY Classes.k");
            static thread_local CacheQuery qu("SELECT atom FROM AClasses WHERE class=?");
            CacheQuery& s = sorted ? qs : qu;
            return s.vec<Atom>(cls.id);
        }
        
        std::vector<Atom>   all_atoms(Document doc, Sorted sorted)
        {
            static thread_local CacheQuery qs("SELECT id FROM Atoms WHERE doc=? ORDER BY Atoms.k");
            static thread_local CacheQuery qu("SELECT id FROM Atoms WHERE doc=?");
            CacheQuery& s = sorted ? qs : qu;
            return s.vec<Atom>(doc.id);
        }

        std::vector<Atom>   all_atoms(Tag tag, Sorted sorted)
        {
            static thread_local CacheQuery qs("SELECT atom FROM ATags INNER JOIN Atoms ON ATags.atom=Atoms.id WHERE tag=? ORDER BY Atoms.k");
            static thread_local CacheQuery qu("SELECT atom FROM ATags WHERE tag=?");
            CacheQuery& s = sorted ? qs : qu;
            return s.vec<Atom>(tag.id);
        }
        
        size_t   count_atoms()
        {
            static thread_local CacheQuery s("SELECT COUNT(1) FROM Atoms");
            return s.size();
        }
        
        size_t   count_atoms(Class cls)
        {
            static thread_local CacheQuery s("SELECT COUNT(1) FROM AClasses WHERE class=?");
            return s.size(cls.id);
        }
        
        size_t   count_atoms(Document doc)
        {
            static thread_local CacheQuery s("SELECT COUNT(1) FROM Atoms WHERE doc=?");
            return s.size(doc);
        }

        size_t   count_atoms(Tag tag)
        {
            static thread_local CacheQuery s("SELECT COUNT(1) FROM ATags WHERE tag=?");
            return s.size(tag.id);
        }
        
        
        //static Atom     create(Document);

        bool     exists_atom(uint64_t i)
        {
            if(!i)
                return false;
                
            static thread_local CacheQuery s("SELECT 1 FROM Atoms WHERE id=?");
            return s.present(i);
        }
        
        Atom     find_atom(uint64_t i)
        {
            return exists_atom(i) ? Atom{i} : Atom{};
        }
        
        Atom     find_atom(std::string_view k)
        {
            static thread_local CacheQuery s("SELECT id FROM Atoms WHERE k=? LIMIT 1");
            return s.as<Atom>(k);
        }
        
        Atom     find_atom(Document doc)
        {
            static thread_local CacheQuery s("SELECT id FROM Atoms WHERE doc=? AND sk=''");
            return s.as<Atom>(doc.id);
        }
        
        Atom     find_atom(Document doc, std::string_view ck)
        {
            static thread_local CacheQuery s("SELECT id FROM Atoms WHERE doc=? AND sk=?");
            return s.as<Atom>(doc.id, ck);
        }

        std::vector<Atom>    named_atoms(std::string_view n, Sorted sorted)
        {
            static thread_local CacheQuery qs("SELECT id FROM Atoms WHERE name=? ORDER BY k");
            static thread_local CacheQuery qu("SELECT id FROM Atoms WHERE name=?");
            CacheQuery& s = sorted ? qs : qu;
            return s.vec<Atom>(n);
        }
        
        // ////////////////////////////////////

        std::string         abbreviation(Atom a) 
        {
            static thread_local CacheQuery s("SELECT abbr FROM Atoms WHERE id=?");
            return s.str(a);
        }
        
        std::string         brief(Atom a) 
        {
            static thread_local CacheQuery s("SELECT brief FROM Atoms WHERE id=?");
            return s.str(a);
        }
        
        std::vector<Atom>   children(Atom a, Sorted sorted) 
        {
            static thread_local CacheQuery qs("SELECT id FROM Atoms WHERE parent=? ORDER BY k");
            static thread_local CacheQuery qu("SELECT id FROM Atoms WHERE parent=?");
            CacheQuery& s = sorted ? qs : qu;
            return s.vec<Atom>(a);
        }
        
        size_t              children_count(Atom a) 
        {
            static thread_local CacheQuery s("SELECT COUNT(1) FROM Atoms WHERE parent=?");
            return s.size(a);
        }

        std::vector<Class>  classes(Atom a, Sorted sorted) 
        {
            static thread_local CacheQuery qs("SELECT class FROM AClasses INNER JOIN Classes ON AClasses.class=Classes.id WHERE atom=? ORDER BY Classes.k");
            static thread_local CacheQuery qu("SELECT class FROM AClasses WHERE atom=?");
            CacheQuery& s = sorted ? qs : qu;
            return s.vec<Class>(a);
        }

        size_t              classes_count(Atom a) 
        {
            static thread_local CacheQuery s("SELECT COUNT(1) FROM AClasses WHERE atom=?");
            return s.size(a);
        }
        
        Document            document(Atom a) 
        {
            static thread_local CacheQuery s("SELECT doc FROM Atoms WHERE id=?");
            return s.as<Document>(a);
        }
        
        Image               icon(Atom a) 
        {
            static thread_local CacheQuery s("SELECT icon FROM Atoms WHERE id=?");
            return s.as<Image>(a);
        }

        Atom::Info          info(Atom a) 
        {
            Atom::Info    ret;
            
            static thread_local CacheQuery s("SELECT k, abbr, brief, name, leaf FROM Atoms WHERE id=?");
            auto s_af = s.af();
            s.bind(1, a.id);
            if(s.step() == SQResult::Row){
                ret.key         = s.v_string(1);
                ret.abbr        = s.v_string(2);
                ret.brief       = s.v_string(3);
                ret.name        = s.v_string(4);
                ret.brief       = Leaf{s.v_uint64(5)};
            }
            return ret;
        }
        
        bool                is(Atom a, Class cls) 
        {
            static thread_local CacheQuery    s("SELECT 1 FROM AClasses WHERE atom=? AND class=?");
            return s.present(a, cls.id);
        }
        
        bool                is(Atom a, Tag tag) 
        {
            static thread_local CacheQuery    s("SELECT 1 FROM ATags WHERE atom=? AND tag=?");
            return s.present(a, tag.id);
        }

        bool                is_edge(Atom a) 
        {
            static thread_local CacheQuery s("SELECT is_edge FROM Atoms WHERE id=?");
            return s.boolean(a);
        }

        bool                is_leaf(Atom a) 
        {
            return leaf(a) != Leaf{};
        }

        std::string         key(Atom a) 
        {
            static thread_local CacheQuery s("SELECT k FROM Atoms WHERE id=?");
            return s.str(a);
        }
        
        std::string         label(Atom a) 
        {
            std::string s   = name(a);
            if(s.empty())
                s   = key(a);
            return s;
        
            //  temporary, fix once able to research
            //static thread_local CacheQuery    s("SELECT ifnull(name,k) FROM Atoms WHERE id=?");
            //return s.str(id);
        }
        
        Leaf                leaf(Atom a) 
        {
            static thread_local CacheQuery s("SELECT leaf FROM Atoms WHERE id=?");
            return s.as<Leaf>(a);
        }
        
        std::string         name(Atom a) 
        {
            static thread_local CacheQuery s("SELECT name FROM Atoms WHERE id=?");
            return s.str(a);
        }
        
        NKI            nki(Atom a, bool autoKey) 
        {
            static thread_local CacheQuery    s("SELECT name,icon,k FROM Atoms WHERE id=?");
            auto s_af = s.af();
            s.bind(1, a.id);
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
            static thread_local CacheQuery s("SELECT parent FROM Atoms WHERE id=?");
            return s.as<Atom>(a);
        }
        
        std::string         skey(Atom a) 
        {
            static thread_local CacheQuery s("SELECT sk FROM Atoms WHERE id=?");
            return s.str(a);
        }
        
        std::vector<Tag>    tags(Atom a, Sorted sorted) 
        {
            static thread_local CacheQuery qs("SELECT tag FROM ATags INNER JOIN Tags ON ATags.tag=Tags.id WHERE atom=? ORDER BY Tags.k");
            static thread_local CacheQuery qu("SELECT tag FROM ATags WHERE atom=?");
            CacheQuery& s = sorted ? qs : qu;
            return s.vec<Tag>(a);
        }

        size_t              tags_count(Atom a) 
        {
            static thread_local CacheQuery s("SELECT COUNT(DISTINCT tag) FROM ATags WHERE atom=?");
            return s.size(a);
        }
        
        std::string         title(Atom a)
        {
            return label(a);
        }
        
        bool                valid(Atom a) 
        {
            return a.id && exists_atom(a.id);
        }

        // ------------------------------------------------------------------------------------------------------------
        // ------------------------------------------------------------------------------------------------------------

        
        std::vector<ClassU64Pair> classes_and_hops(Atom a) 
        {
            std::vector<ClassU64Pair> ret;

            static thread_local CacheQuery s("SELECT class, hops FROM AClasses WHERE atom=?");
            auto s_af           = s.af();
            
            s.bind(1, a.id);
            
            while(s.step() == SQResult::Row)
                ret.push_back(ClassU64Pair{Class{s.v_uint64(1)}, s.v_uint64(2)});
            
            return ret;
        }

        ClassCountMap               classes_and_hops_map(Atom a)
        {
            ClassCountMap   ret;
            static thread_local CacheQuery s("SELECT class, hops FROM AClasses WHERE atom=?");
            auto s_af           = s.af();
            
            s.bind(1, a.id);
            
            while(s.step() == SQResult::Row)
                ret[Class{s.v_uint64(1)}] = { s.v_uint64(2)};
            
            return ret;
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

            static thread_local CacheQuery    i("INSERT OR FAIL INTO Atoms (k,doc,sk) VALUES (?,?,?)");
            static thread_local CacheQuery    s("SELECT id FROM Atoms WHERE k=?");
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
}
