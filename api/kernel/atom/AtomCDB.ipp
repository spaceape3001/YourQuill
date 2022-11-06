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
    std::vector<Atom>   Atom::all(Sorted sorted)
    {
        static thread_local cdb::SQ qs("SELECT id FROM Atoms ORDER BY k");
        static thread_local cdb::SQ qu("SELECT id FROM Atoms");
        cdb::SQ& s = sorted ? qs : qu;
        return s.vec<Atom>();
    }
    
    std::vector<Atom>   Atom::all(Class cls, Sorted sorted)
    {
                // I think this SQL is right.....
        static thread_local cdb::SQ qs("SELECT atom FROM AClasses INNER JOIN Classes ON AClasses.class=Classes.id WHERE class=? ORDER BY Classes.k");
        static thread_local cdb::SQ qu("SELECT atom FROM AClasses WHERE class=?");
        cdb::SQ& s = sorted ? qs : qu;
        return s.vec<Atom>(cls.id);
    }
    
    std::vector<Atom>   Atom::all(Document doc, Sorted sorted)
    {
        static thread_local cdb::SQ qs("SELECT id FROM Atoms WHERE doc=? ORDER BY Atoms.k");
        static thread_local cdb::SQ qu("SELECT id FROM Atoms WHERE doc=?");
        cdb::SQ& s = sorted ? qs : qu;
        return s.vec<Atom>(doc.id);
    }

    std::vector<Atom>   Atom::all(Tag tag, Sorted sorted)
    {
        static thread_local cdb::SQ qs("SELECT atom FROM ATags INNER JOIN Tags ON ATags.atom=Atoms.id WHERE tag=? ORDER BY Atoms.k");
        static thread_local cdb::SQ qu("SELECT atom FROM ATags WHERE tag=?");
        cdb::SQ& s = sorted ? qs : qu;
        return s.vec<Atom>(tag.id);
    }
    
    size_t   Atom::count()
    {
        static thread_local cdb::SQ s("SELECT COUNT(1) FROM Atoms");
        return s.size();
    }
    
    size_t   Atom::count(Class cls)
    {
        static thread_local cdb::SQ s("SELECT COUNT(1) FROM AClasses WHERE class=?");
        return s.size(cls.id);
    }
    
    size_t   Atom::count(Document doc)
    {
        static thread_local cdb::SQ s("SELECT COUNT(1) FROM Atoms WHERE doc=?");
        return s.size(doc);
    }

    size_t   Atom::count(Tag tag)
    {
        static thread_local cdb::SQ s("SELECT COUNT(1) FROM ATags WHERE tag=?");
        return s.size(tag.id);
    }
    
    
    //static Atom     create(Document);

    bool     Atom::exists(uint64_t i)
    {
        if(!i)
            return false;
            
        static thread_local cdb::SQ s("SELECT 1 FROM Atoms WHERE id=?");
        return s.present(i);
    }
    
    Atom     Atom::find(uint64_t i)
    {
        return exists(i) ? Atom{i} : Atom{};
    }
    
    Atom     Atom::find(std::string_view k)
    {
        static thread_local cdb::SQ s("SELECT id FROM Atoms WHERE k=? LIMIT 1");
        return s.as<Atom>(k);
    }
    
    Atom     Atom::find(Document doc)
    {
        static thread_local cdb::SQ s("SELECT id FROM Atoms WHERE doc=? AND sk=''");
        return s.as<Atom>(doc.id);
    }
    
    Atom     Atom::find(Document doc, std::string_view ck)
    {
        std::string ks  = full_key(doc, ck);
        return find(ks);
    }

    std::string  Atom::full_key(Document doc, std::string_view ck)
    {
        std::string     kk  = cdb::key(doc);
        std::string     dk = std::string(strip_extension(kk));
        if(dk.empty())    
            return std::string();

        if(!ck.empty()){
            dk += '#';
            dk += ck;
        }
        return dk;
    }
    
    std::vector<Atom>    Atom::named(std::string_view n, Sorted sorted)
    {
        static thread_local cdb::SQ qs("SELECT id FROM Atoms WHERE name=? ORDER BY k");
        static thread_local cdb::SQ qu("SELECT id FROM Atoms WHERE name=?");
        cdb::SQ& s = sorted ? qs : qu;
        return s.vec<Atom>(n);
    }
    
    // ////////////////////////////////////

    std::string         Atom::abbreviation() const
    {
        static thread_local cdb::SQ s("SELECT abbr FROM Atoms WHERE id=?");
        return s.str(id);
    }
    
    std::string         Atom::brief() const
    {
        static thread_local cdb::SQ s("SELECT brief FROM Atoms WHERE id=?");
        return s.str(id);
    }
    
    std::vector<Atom>   Atom::children(Sorted sorted) const
    {
        static thread_local cdb::SQ qs("SELECT id FROM Atoms WHERE parent=? ORDER BY k");
        static thread_local cdb::SQ qu("SELECT id FROM Atoms WHERE parent=?");
        cdb::SQ& s = sorted ? qs : qu;
        return s.vec<Atom>(id);
    }
    
    size_t              Atom::children_count() const
    {
        static thread_local cdb::SQ s("SELECT COUNT(1) FROM Atoms WHERE parent=?");
        return s.size(id);
    }

    std::vector<Class>  Atom::classes(Sorted sorted) const
    {
        static thread_local cdb::SQ qs("SELECT class FROM AClasses INNER JOIN Classes ON AClasses.class=Classes.id WHERE atom=? ORDER BY Classes.k");
        static thread_local cdb::SQ qu("SELECT class FROM AClasses WHERE atom=?");
        cdb::SQ& s = sorted ? qs : qu;
        return s.vec<Class>(id);
    }

    size_t              Atom::classes_count() const
    {
        static thread_local cdb::SQ s("SELECT COUNT(1) FROM AClasses WHERE atom=?");
        return s.size(id);
    }
    
    Document            Atom::document() const
    {
        static thread_local cdb::SQ s("SELECT doc FROM Atoms WHERE id=?");
        return s.as<Document>(id);
    }
    
    Image               Atom::icon() const
    {
        static thread_local cdb::SQ s("SELECT icon FROM Atoms WHERE id=?");
        return s.as<Image>(id);
    }

    Atom::Info          Atom::info() const
    {
        Info    ret;
        
        static thread_local cdb::SQ s("SELECT k, abbr, brief, name, leaf FROM Atoms WHERE id=?");
        auto s_af = s.af();
        s.bind(1, id);
        if(s.step() == SqlQuery::Row){
            ret.key         = s.v_string(1);
            ret.abbr        = s.v_string(2);
            ret.brief       = s.v_string(3);
            ret.name        = s.v_string(4);
            ret.brief       = Leaf{s.v_uint64(5)};
        }
        return ret;
    }
    
    bool                Atom::is(Class cls) const
    {
        static thread_local cdb::SQ    s("SELECT 1 FROM AClasses WHERE atom=? AND class=?");
        return s.present(id, cls.id);
    }
    
    bool                Atom::is(Tag tag) const
    {
        static thread_local cdb::SQ    s("SELECT 1 FROM ATags WHERE atom=? AND tag=?");
        return s.present(id, tag.id);
    }

    bool                Atom::is_edge() const
    {
        static thread_local cdb::SQ s("SELECT is_edge FROM Atoms WHERE id=?");
        return s.boolean(id);
    }

    bool                Atom::is_leaf() const
    {
        return leaf() != Leaf{};
    }

    std::string         Atom::key() const
    {
        static thread_local cdb::SQ s("SELECT k FROM Atoms WHERE id=?");
        return s.str(id);
    }
    
    std::string         Atom::label() const
    {
        std::string s   = name();
        if(s.empty())
            s   = key();
        return s;
    
        //  temporary, fix once able to research
        //static thread_local SQ    s("SELECT ifnull(name,k) FROM Atoms WHERE id=?");
        //return s.str(id);
    }
    
    Leaf                Atom::leaf() const
    {
        static thread_local cdb::SQ s("SELECT leaf FROM Atoms WHERE id=?");
        return s.as<Leaf>(id);
    }
    
    std::string         Atom::name() const
    {
        static thread_local cdb::SQ s("SELECT name FROM Atoms WHERE id=?");
        return s.str(id);
    }
    
    cdb::NKI            Atom::nki(bool autoKey) const
    {
        static thread_local cdb::SQ    s("SELECT name,icon,k FROM Atoms WHERE id=?");
        auto s_af = s.af();
        s.bind(1, id);
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

    Atom                Atom::parent() const
    {
        static thread_local cdb::SQ s("SELECT parent FROM Atoms WHERE id=?");
        return s.as<Atom>(id);
    }
    
    std::string         Atom::skey() const
    {
        static thread_local cdb::SQ s("SELECT sk FROM Atoms WHERE id=?");
        return s.str(id);
    }
    
    std::vector<Tag>    Atom::tags(Sorted sorted) const
    {
        static thread_local cdb::SQ qs("SELECT tag FROM ATags INNER JOIN Tags ON ATags.tag=Tags.id WHERE atom=? ORDER BY Tags.k");
        static thread_local cdb::SQ qu("SELECT tag FROM ATags WHERE atom=?");
        cdb::SQ& s = sorted ? qs : qu;
        return s.vec<Tag>(id);
    }

    size_t              Atom::tags_count() const
    {
        static thread_local cdb::SQ s("SELECT COUNT(DISTINCT tag) FROM ATags WHERE atom=?");
        return s.size(id);
    }
    
    std::string         Atom::title() const
    {
        return label();
    }
    
    bool                Atom::valid() const
    {
        return id && exists(id);
    }

    namespace cdb {


        std::string             abbreviation(Atom a)
        {
            return a.abbreviation();
        }

        std::vector<Atom>        all_atoms(Sorted sorted)
        {
            return Atom::all(sorted);
        }
        
        std::vector<Atom>        all_atoms(Class c,Sorted sorted)
        {
            return Atom::all(c, sorted);
        }
        
        std::vector<Atom>        all_atoms(Tag t,Sorted sorted)
        {
            return Atom::all(t, sorted);
        }
        
        size_t              all_atoms_count()
        {
            return Atom::count();
        }
        
        size_t              all_atoms_count(Class c)
        {
            return Atom::count(c);
        }
        
        size_t              all_atoms_count(Tag t)
        {
            return Atom::count(t);
        }
        
        Atom                atom(uint64_t i)
        {
            return Atom::find(i);
        }
        
        Atom                atom(std::string_view  k)
        {
            return Atom::find(k);
        }

        Atom                atom(Document doc, std::string_view  ck)
        {
            return Atom::find(doc, ck);
        }
        
        std::vector<Atom>       atoms(Class cls, Sorted sorted)
        {
            return Atom::all(cls, sorted);
        }

        std::vector<Atom>        atoms(Document doc, Sorted sorted)
        {
            return Atom::all(doc, sorted);
        }
        
        std::vector<Atom>             atoms_by_name(std::string_view  n, Sorted sorted)
        {
            return Atom::named(n, sorted);
        }

        size_t              atoms_count(Document doc)
        {
            return Atom::count(doc);
        }
        
        std::string             brief(Atom a)
        {
            return a.brief();
        }

        std::vector<Atom>            child_atoms(Atom a, Sorted sorted)
        {
            return a.children(sorted);
        }

        size_t              child_atoms_count(Atom a)
        {
            return a.children_count();
        }
        
        std::vector<Class>       classes(Atom a, Sorted sorted)
        {
            return a.classes(sorted);
        }
        
        //std::vector<Class>       classes(Atom a, Document d, Sorted sorted)
        //{
            //static thread_local SQ qs("SELECT class FROM AClasses INNER JOIN Classes ON AClasses.class=Classes.id WHERE atom=? AND doc=? ORDER BY Classes.k");
            //static thread_local SQ qu("SELECT class FROM AClasses WHERE atom=? AND doc=?");
            //SQ& s = sorted ? qs : qu;
            //return s.vec<Class>(a.id, d.id);
        //}
        
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

        size_t              classes_count(Atom a)
        {
            return a.classes_count();
        }    
        
        //size_t              classes_count(Atom a, Document d)
        //{
            //static thread_local SQ s("SELECT COUNT(1) FROM AClasses WHERE atom=? AND doc=?");
            //return s.size(a.id, d.id);
        //}    


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
        
        bool                exists(Atom a)
        {
            return a.valid();
        }
        
        bool                exists_atom(uint64_t i)
        {
            return Atom::exists(i);
        }

       
        Image               icon(Atom a) 
        {
            return a.icon();
        }
        
        std::vector<Atom>   inbound(Atom a)
        {
            static thread_local SQ s("SELECT edge FROM AEdges WHERE target=?");
            return s.vec<Atom>(a.id);
        }

        Atom::Info          info(Atom a)
        {
            return a.info();
        }
        
        
        bool                is(Atom a, Class c)
        {
            return a.is(c);
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
        
        bool                is_tagged(Atom a, Tag t)
        {
            return a.is(t);
        }
        
     
        std::string             key(Atom a)
        {
            return a.key();
        }
        
        std::string             label(Atom a)
        {
            return a.label();
        }
        
        Leaf                    leaf(Atom a)
        {
            return a.leaf();
        }

        std::string             name(Atom a)
        {
            return a.name();
        }

        
        NKI                 nki(Atom a, bool autoKey)
        {
            return a.nki(autoKey);
        }
        

        std::vector<Atom>   outbound(Atom a)
        {
            static thread_local SQ s("SELECT target FROM AEdges WHERE atom=? AND tgtid!=0");
            return s.vec<Atom>(a.id);
        }

        Atom                parent(Atom a)
        {
            return a.parent();
        }
        

        std::string             skey(Atom a) 
        {
            return a.skey();
        }
        
        std::vector<Tag>              tags(Atom a, Sorted sorted)
        {   
            return a.tags(sorted);
        }
        
        size_t              tags_count(Atom a)
        {
            return a.tags_count();
        }

        std::string             title(Atom a)
        {
            return a.title();
        }
    }
}
