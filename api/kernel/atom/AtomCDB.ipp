////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AtomCDB.hpp"
#include <basic/TextUtils.hpp>
#include <kernel/atom/Class.hpp>
#include <kernel/db/NKI.hpp>
#include <kernel/db/SQ.hpp>
#include <kernel/file/DocumentCDB.hpp>
#include <kernel/image/Image.hpp>
#include <kernel/org/Tag.hpp>

namespace yq {
    namespace cdb {
    
        std::vector<Atom>   all_atoms(Sorted sorted)
        {
            static thread_local SQ qs("SELECT id FROM Atoms ORDER BY k");
            static thread_local SQ qu("SELECT id FROM Atoms");
            SQ& s = sorted ? qs : qu;
            return s.vec<Atom>();
        }
        
        std::vector<Atom>   all_atoms(Class cls, Sorted sorted)
        {
                    // I think this SQL is right.....
            static thread_local SQ qs("SELECT atom FROM AClasses INNER JOIN Classes ON AClasses.class=Classes.id WHERE class=? ORDER BY Classes.k");
            static thread_local SQ qu("SELECT atom FROM AClasses WHERE class=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<Atom>(cls.id);
        }
        
        std::vector<Atom>   all_atoms(Document doc, Sorted sorted)
        {
            static thread_local SQ qs("SELECT id FROM Atoms WHERE doc=? ORDER BY Atoms.k");
            static thread_local SQ qu("SELECT id FROM Atoms WHERE doc=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<Atom>(doc.id);
        }

        std::vector<Atom>   all_atoms(Tag tag, Sorted sorted)
        {
            static thread_local SQ qs("SELECT atom FROM ATags INNER JOIN Tags ON ATags.atom=Atoms.id WHERE tag=? ORDER BY Atoms.k");
            static thread_local SQ qu("SELECT atom FROM ATags WHERE tag=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<Atom>(tag.id);
        }
        
        size_t   count_atoms()
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Atoms");
            return s.size();
        }
        
        size_t   count_atoms(Class cls)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM AClasses WHERE class=?");
            return s.size(cls.id);
        }
        
        size_t   count_atoms(Document doc)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Atoms WHERE doc=?");
            return s.size(doc);
        }

        size_t   count_atoms(Tag tag)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM ATags WHERE tag=?");
            return s.size(tag.id);
        }
        
        
        //static Atom     create(Document);

        bool     exists_atom(uint64_t i)
        {
            if(!i)
                return false;
                
            static thread_local SQ s("SELECT 1 FROM Atoms WHERE id=?");
            return s.present(i);
        }
        
        Atom     find_atom(uint64_t i)
        {
            return exists_atom(i) ? Atom{i} : Atom{};
        }
        
        Atom     find_atom(std::string_view k)
        {
            static thread_local SQ s("SELECT id FROM Atoms WHERE k=? LIMIT 1");
            return s.as<Atom>(k);
        }
        
        Atom     find_atom(Document doc)
        {
            static thread_local SQ s("SELECT id FROM Atoms WHERE doc=? AND sk=''");
            return s.as<Atom>(doc.id);
        }
        
        Atom     find_atom(Document doc, std::string_view ck)
        {
            static thread_local SQ s("SELECT id FROM Atoms WHERE doc=? AND sk=?");
            return s.as<Atom>(doc.id, ck);
        }

        std::vector<Atom>    named_atoms(std::string_view n, Sorted sorted)
        {
            static thread_local SQ qs("SELECT id FROM Atoms WHERE name=? ORDER BY k");
            static thread_local SQ qu("SELECT id FROM Atoms WHERE name=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<Atom>(n);
        }
        
        // ////////////////////////////////////

        std::string         abbreviation(Atom a) 
        {
            static thread_local SQ s("SELECT abbr FROM Atoms WHERE id=?");
            return s.str(a);
        }
        
        std::string         brief(Atom a) 
        {
            static thread_local SQ s("SELECT brief FROM Atoms WHERE id=?");
            return s.str(a);
        }
        
        std::vector<Atom>   children(Atom a, Sorted sorted) 
        {
            static thread_local SQ qs("SELECT id FROM Atoms WHERE parent=? ORDER BY k");
            static thread_local SQ qu("SELECT id FROM Atoms WHERE parent=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<Atom>(a);
        }
        
        size_t              children_count(Atom a) 
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Atoms WHERE parent=?");
            return s.size(a);
        }

        std::vector<Class>  classes(Atom a, Sorted sorted) 
        {
            static thread_local SQ qs("SELECT class FROM AClasses INNER JOIN Classes ON AClasses.class=Classes.id WHERE atom=? ORDER BY Classes.k");
            static thread_local SQ qu("SELECT class FROM AClasses WHERE atom=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<Class>(a);
        }

        size_t              classes_count(Atom a) 
        {
            static thread_local SQ s("SELECT COUNT(1) FROM AClasses WHERE atom=?");
            return s.size(a);
        }
        
        Document            document(Atom a) 
        {
            static thread_local SQ s("SELECT doc FROM Atoms WHERE id=?");
            return s.as<Document>(a);
        }
        
        Image               icon(Atom a) 
        {
            static thread_local SQ s("SELECT icon FROM Atoms WHERE id=?");
            return s.as<Image>(a);
        }

        Atom::Info          info(Atom a) 
        {
            Atom::Info    ret;
            
            static thread_local SQ s("SELECT k, abbr, brief, name, leaf FROM Atoms WHERE id=?");
            auto s_af = s.af();
            s.bind(1, a.id);
            if(s.step() == SqlQuery::Row){
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
            static thread_local SQ    s("SELECT 1 FROM AClasses WHERE atom=? AND class=?");
            return s.present(a, cls.id);
        }
        
        bool                is(Atom a, Tag tag) 
        {
            static thread_local SQ    s("SELECT 1 FROM ATags WHERE atom=? AND tag=?");
            return s.present(a, tag.id);
        }

        bool                is_edge(Atom a) 
        {
            static thread_local SQ s("SELECT is_edge FROM Atoms WHERE id=?");
            return s.boolean(a);
        }

        bool                is_leaf(Atom a) 
        {
            return leaf(a) != Leaf{};
        }

        std::string         key(Atom a) 
        {
            static thread_local SQ s("SELECT k FROM Atoms WHERE id=?");
            return s.str(a);
        }
        
        std::string         label(Atom a) 
        {
            std::string s   = name(a);
            if(s.empty())
                s   = key(a);
            return s;
        
            //  temporary, fix once able to research
            //static thread_local SQ    s("SELECT ifnull(name,k) FROM Atoms WHERE id=?");
            //return s.str(id);
        }
        
        Leaf                leaf(Atom a) 
        {
            static thread_local SQ s("SELECT leaf FROM Atoms WHERE id=?");
            return s.as<Leaf>(a);
        }
        
        std::string         name(Atom a) 
        {
            static thread_local SQ s("SELECT name FROM Atoms WHERE id=?");
            return s.str(a);
        }
        
        cdb::NKI            nki(Atom a, bool autoKey) 
        {
            static thread_local SQ    s("SELECT name,icon,k FROM Atoms WHERE id=?");
            auto s_af = s.af();
            s.bind(1, a.id);
            if(s.step() == SqlQuery::Row){
                cdb::NKI  ret;
                ret.name    = s.v_string(1);
                ret.icon    = Image{s.v_uint64(2)};
                ret.key     = s.v_string(3);
                if(autoKey && ret.name.empty())
                    ret.name    = ret.key;
                return ret;
            }
            return cdb::NKI();
        }

        Atom                parent(Atom a) 
        {
            static thread_local SQ s("SELECT parent FROM Atoms WHERE id=?");
            return s.as<Atom>(a);
        }
        
        std::string         skey(Atom a) 
        {
            static thread_local SQ s("SELECT sk FROM Atoms WHERE id=?");
            return s.str(a);
        }
        
        std::vector<Tag>    tags(Atom a, Sorted sorted) 
        {
            static thread_local SQ qs("SELECT tag FROM ATags INNER JOIN Tags ON ATags.tag=Tags.id WHERE atom=? ORDER BY Tags.k");
            static thread_local SQ qu("SELECT tag FROM ATags WHERE atom=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<Tag>(a);
        }

        size_t              tags_count(Atom a) 
        {
            static thread_local SQ s("SELECT COUNT(DISTINCT tag) FROM ATags WHERE atom=?");
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

            static thread_local SQ s("SELECT class, hops FROM AClasses WHERE atom=?");
            auto s_af           = s.af();
            
            s.bind(1, a.id);
            
            while(s.step() == SqlQuery::Row)
                ret.push_back(ClassU64Pair{Class{s.v_uint64(1)}, s.v_uint64(2)});
            
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

            static thread_local SQ    i("INSERT OR FAIL INTO Atoms (k,doc,sk) VALUES (?,?,?)");
            static thread_local SQ    s("SELECT id FROM Atoms WHERE k=?");
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
                if(s.step() == SqlQuery::Row)
                    return Atom{s.v_uint64(1)};
                yError() << "Unable to get the atom from the database: " << dk;
                return Atom();
            }
        }
        
        std::vector<Atom>   inbound(Atom a)
        {
            static thread_local SQ s("SELECT edge FROM AEdges WHERE target=?");
            return s.vec<Atom>(a.id);
        }


        std::vector<Atom>   outbound(Atom a)
        {
            static thread_local SQ s("SELECT target FROM AEdges WHERE atom=? AND tgtid!=0");
            return s.vec<Atom>(a.id);
        }
    }
}
