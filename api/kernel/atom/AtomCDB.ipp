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
        namespace {
            std::string     full_key(Document d, std::string_view  ck)
            {
                std::string     kk  = key(d);
                std::string     dk = std::string(strip_extension(kk));
                if(dk.empty())    
                    return std::string();

                if(!ck.empty()){
                    dk += '#';
                    dk += ck;
                }
                return dk;
            }
        }


        std::string             abbreviation(Atom a)
        {
            static thread_local SQ s("SELECT abbr FROM Atoms WHERE id=?");
            return s.str(a.id);
        }

        std::vector<Atom>        all_atoms(Sorted sorted)
        {
            static thread_local SQ qs("SELECT id FROM Atoms ORDER BY k");
            static thread_local SQ qu("SELECT id FROM Atoms");
            SQ& s = sorted ? qs : qu;
            return s.vec<Atom>();
        }
        
        std::vector<Atom>        all_atoms(Class c,Sorted sorted)
        {
                    // I think this SQL is right.....
            static thread_local SQ qs("SELECT atom FROM AClasses INNER JOIN Classes ON AClasses.class=Classes.id WHERE class=? ORDER BY Classes.k");
            static thread_local SQ qu("SELECT atom FROM AClasses WHERE class=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<Atom>(c.id);
        }
        
        std::vector<Atom>        all_atoms(Tag t,Sorted sorted)
        {
            static thread_local SQ qs("SELECT atom FROM ATags INNER JOIN Tags ON ATags.tag=Tags.id WHERE tag=? ORDER BY Tags.k");
            static thread_local SQ qu("SELECT atom FROM ATags WHERE tag=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<Atom>(t.id);
        }
        
        size_t              all_atoms_count()
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Atoms");
            return s.size();
        }
        
        size_t              all_atoms_count(Class c)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM AClasses WHERE class=?");
            return s.size(c.id);
        }
        
        size_t              all_atoms_count(Tag t)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM ATags WHERE tag=?");
            return s.size(t.id);
        }
        
        
        Atom                atom(uint64_t i)
        {
            return exists_atom(i) ? Atom{i} : Atom();
        }
        
        Atom                atom(std::string_view  k)
        {
            static thread_local SQ s("SELECT id FROM Atoms WHERE k=? LIMIT 1");
            return s.as<Atom>(k);
        }

        Atom                atom(Document doc, std::string_view  ck)
        {
            std::string ks  = full_key(doc, ck);
            return atom(ks);
        }
        
        std::vector<Atom>       atoms(Class cls, Sorted sorted)
        {
            static thread_local SQ qs("SELECT DISTINCT atom FROM AClasses INNER JOIN Atoms ON AClasses.atom=Atoms.id WHERE class=? ORDER BY Atoms.k");
            static thread_local SQ qu("SELECT DISTINCT atom FROM AClasses WHERE class=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<Atom>(cls.id);
        }

        std::vector<Atom>        atoms(Document doc, Sorted sorted)
        {
            static thread_local SQ qs("SELECT atom FROM ADocuments INNER JOIN Atoms ON ADocuments.atom=Atoms.id WHERE doc=? ORDER BY Atoms.k");
            static thread_local SQ qu("SELECT atom FROM ADocuments WHERE doc=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<Atom>(doc.id);
        }
        
        std::vector<Atom>             atoms_by_name(std::string_view  n, Sorted sorted)
        {
            static thread_local SQ qs("SELECT id FROM Atoms WHERE name=? ORDER BY k");
            static thread_local SQ qu("SELECT id FROM Atoms WHERE name=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<Atom>(n);
        }

        size_t              atoms_count(Document doc)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Atoms WHERE doc=?");
            return s.size(doc);
        }
        
        std::string             brief(Atom a)
        {
            static thread_local SQ s("SELECT brief FROM Atoms WHERE id=?");
            return s.str(a.id);
        }

        std::vector<Atom>            child_atoms(Atom a, Sorted sorted)
        {
            static thread_local SQ qs("SELECT id FROM Atoms WHERE parent=? ORDER BY k");
            static thread_local SQ qu("SELECT id FROM Atoms WHERE parent=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<Atom>(a.id);
        }

        size_t              child_atoms_count(Atom a)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Atoms WHERE parent=?");
            return s.size(a);
        }
        
        std::vector<Class>       classes(Atom a, Sorted sorted)
        {
            static thread_local SQ qs("SELECT DISTINCT class FROM AClasses INNER JOIN Classes ON AClasses.class=Classes.id WHERE atom=? ORDER BY Classes.k");
            static thread_local SQ qu("SELECT DISTINCT class FROM AClasses WHERE atom=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<Class>(a.id);
        }
        
        std::vector<Class>       classes(Atom a, Document d, Sorted sorted)
        {
            static thread_local SQ qs("SELECT class FROM AClasses INNER JOIN Classes ON AClasses.class=Classes.id WHERE atom=? AND doc=? ORDER BY Classes.k");
            static thread_local SQ qu("SELECT class FROM AClasses WHERE atom=? AND doc=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<Class>(a.id, d.id);
        }
        
        std::vector<ClassU64Pair> classes_and_hops(Atom a, Document d) 
        {
            std::vector<ClassU64Pair> ret;

            static thread_local SQ s("SELECT class, hops FROM AClasses WHERE atom=? AND doc=?");
            auto s_af           = s.af();
            
            s.bind(1, a.id);
            s.bind(2, d.id);
            
            while(s.step() == SqlQuery::Row)
                ret.push_back(ClassU64Pair{Class{s.v_uint64(1)}, s.v_uint64(2)});
            
            return ret;
        }

        size_t              classes_count(Atom a)
        {
            static thread_local SQ s("SELECT COUNT(DISTINCT class) FROM AClasses WHERE atom=?");
            return s.size(a.id);
        }    
        
        size_t              classes_count(Atom a, Document d)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM AClasses WHERE atom=? AND doc=?");
            return s.size(a.id, d.id);
        }    


        Atom                db_atom(Document d, bool* wasCreated)
        {
            return db_atom(d, std::string(), wasCreated);
        }
        
        Atom                db_atom(Document d, std::string_view ck, bool* wasCreated)
        {
            if(wasCreated)
                *wasCreated = false;


            std::string     dk = full_key(d, ck);
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
        
        //Document            document(Atom a)
        //{
            //static thread_local SQ s("SELECT doc FROM Atoms WHERE id=?");
            //auto s_af = s.af();
            //s.bind(0, a.id);
            //if(s.exec() && s.next())
                //return Document{s.valueU64(0)};
            //return Document();
        //}
        
        namespace {
            std::vector<Document>  documents_sorted(Atom a)
            {
                static thread_local SQ  s("SELECT doc FROM ADocuments INNER JOIN Documents ON ADocuments.doc=Documents.id WHERE id=? ORDER BY Documents.k");
                return s.vec<Document>(a.id);
            }
            
            std::vector<Document>  documents_unsorted(Atom a)
            {
                static thread_local SQ  s("SELECT doc FROM ADocuments WHERE id=?");
                return s.vec<Document>(a.id);
            }
            
        }
        
        std::vector<Document>        documents(Atom a, Sorted sorted)
        {
            return sorted ? documents_sorted(a) : documents_unsorted(a);
        }
        
        size_t                  documents_count(Atom a)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM ADocuments WHERE id=?");
            return s.size(a.id);
        }

       bool                exists(Atom a)
        {
            return exists_atom(a.id);
        }
        
       
        bool                exists_atom(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Atoms WHERE id=?");
            return s.present(i);
        }

        
       
        Image               icon(Atom a) 
        {
            static thread_local SQ s("SELECT icon FROM Atoms WHERE id=?");
            return s.as<Image>(a.id);
        }
        
        std::vector<Atom>   inbound(Atom a)
        {
            static thread_local SQ s("SELECT atom FROM AEdges WHERE tgtid=?");
            return s.vec<Atom>(a.id);
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
        
        
        bool                is(Atom a, Class c)
        {
            static thread_local SQ    s("SELECT 1 FROM AClasses WHERE atom=? AND class=?");
            return s.present(a.id, c.id);
        }
        
        //bool                is_all(Atom a, std::initializer_list<Class>cs)
        //{
            //for(Class c : cs)
                //if(!is(a, c))
                    //return false;
            //return true;
        //}
        
        //bool                is_any(Atom a, std::initializer_list<Class>cs)
        //{
            //for(Class c : cs)
                //if(is(a, c))
                    //return true;
            //return false;
        //}
        
     
        std::string             key(Atom a)
        {
            static thread_local SQ s("SELECT k FROM Atoms WHERE id=?");
            return s.str(a.id);
        }
        
        std::string             label(Atom a)
        {
            std::string s   = name(a);
            if(s.empty())
                s   = key(a);
            return s;
        
            //  temporary, fix once able to research
            //static thread_local SQ    s("SELECT ifnull(name,k) FROM Atoms WHERE id=?");
            //return s.str(a.id);
        }
        
        
        Leaf                    leaf(Atom a)
        {
            static thread_local SQ s("SELECT leaf FROM Atoms WHERE id=?");
            return s.as<Leaf>(a.id);
        }

        std::string             name(Atom a)
        {
            static thread_local SQ s("SELECT name FROM Atoms WHERE id=?");
            return s.str(a.id);
        }

        
        NKI                 nki(Atom a, bool autoKey)
        {
            static thread_local SQ    s("SELECT name,icon,k FROM Atoms WHERE id=?");
            auto s_af = s.af();
            s.bind(1, a.id);
            if(s.step() == SqlQuery::Row){
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
        

        std::vector<Atom>   outbound(Atom a)
        {
            static thread_local SQ s("SELECT target FROM AEdges WHERE atom=? AND tgtid!=0");
            return s.vec<Atom>(a.id);
        }

        Atom                parent(Atom a)
        {
            static thread_local SQ s("SELECT parent FROM Atoms WHERE id=?");
            return s.as<Atom>(a.id);
        }
        

        std::string             skey(Atom a) 
        {
            static thread_local SQ s("SELECT sk FROM Atoms WHERE id=?");
            return s.str(a.id);
        }
        
        std::vector<Tag>              tags(Atom a, Sorted sorted)
        {
            static thread_local SQ qs("SELECT DISTINCT tag FROM ATags INNER JOIN Tags ON ATags.tag=Tags.id WHERE atom=? ORDER BY Tags.k");
            static thread_local SQ qu("SELECT DISTINCT tag FROM ATags WHERE atom=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<Tag>(a.id);
        }
        
        std::vector<Tag>              tags(Atom a, Document d, Sorted sorted)
        {
            static thread_local SQ qs("SELECT tag FROM ATags INNER JOIN Tags ON ATags.tag=Tags.id WHERE atom=? AND doc=? ORDER BY Tags.k");
            static thread_local SQ qu("SELECT tag FROM ATags WHERE atom=? AND doc=?");
            SQ& s = sorted ? qs : qu;
            return s.vec<Tag>(a.id,d.id);
        }

        bool                tagged(Atom a, Tag t)
        {
            static thread_local SQ s("SELECT 1 FROM ATags WHERE atom=? AND tag=? LIMIT 1");
            return s.present(a.id, t.id);
        }
        
        size_t              tags_count(Atom a)
        {
            static thread_local SQ s("SELECT COUNT(DISTINCT tag) FROM ATags WHERE atom=?");
            return s.size(a.id);
        }

        size_t              tags_count(Atom a, Document d)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM ATags WHERE atom=? AND doc=?");
            return s.size(a.id, d.id);
        }

        std::string             title(Atom a)
        {
            return label(a);
        }
    }
}
