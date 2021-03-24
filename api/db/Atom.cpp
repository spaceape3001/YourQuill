////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Atom.hpp"
#include "Class.hpp"

#include "Root.hpp"
#include "Workspace.hpp"
#include "Graph.hpp"
#include "Image.hpp"
#include "Tag.hpp"

#include <db/bit/NKI.hpp>
#include <util/Logging.hpp>
#include <util/SqlQuery.hpp>
#include <util/Utilities.hpp>

#include <QSqlError>

namespace cdb {

    QString             abbreviation(Atom a)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT abbr FROM Atoms WHERE id=?");
        auto s_af = s.af();
        s.bind(0, a.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }

    namespace {
        Vector<Atom>        all_atoms_sorted()
        {
            Vector<Atom>    ret;
            static thread_local SqlQuery s(wksp::cache(), "SELECT id FROM Atoms ORDER BY k");
            auto s_af = s.af();
            if(s.exec()){
                while(s.next())
                    ret << Atom{s.valueU64(0)};
            }
            return ret;
        }

        Vector<Atom>        all_atoms_unsorted()
        {
            Vector<Atom>    ret;
            static thread_local SqlQuery s(wksp::cache(), "SELECT id FROM Atoms");
            auto s_af = s.af();
            if(s.exec()){
                while(s.next())
                    ret << Atom{s.valueU64(0)};
            }
            return ret;
        }
    }
    

    Vector<Atom>        all_atoms(Sorted sorted)
    {
        return sorted ? all_atoms_sorted() : all_atoms_unsorted();
    }
    
    namespace {
        Vector<Atom>    all_atoms_sorted(Class c)
        {
            Vector<Atom>    ret;
                    // I think this SQL is right.....
            static thread_local SqlQuery s(wksp::cache(), "SELECT atom FROM AClasses INNER JOIN Classes ON AClasses.class=Classes.id "
                                "WHERE class=? ORDER BY Classes.k");
            auto s_af = s.af();
            s.bind(0, c.id);
            if(s.exec()){
                while(s.next())
                    ret << Atom{s.valueU64(0)};
            }
            return ret;
        }

        Vector<Atom>    all_atoms_unsorted(Class c)
        {
            Vector<Atom>    ret;
            static thread_local SqlQuery s(wksp::cache(), "SELECT atom FROM AClasses WHERE class=?");
            auto s_af = s.af();
            s.bind(0, c.id);
            if(s.exec()){
                while(s.next())
                    ret << Atom{s.valueU64(0)};
            }
            return ret;
        }
    }
    
    Vector<Atom>        all_atoms(Class c,Sorted sorted)
    {
        return sorted ? all_atoms_sorted(c) : all_atoms_unsorted(c);
    }
    
    namespace {
        Vector<Atom>    all_atoms_sorted(Tag t)
        {
            Vector<Atom>    ret;
            static thread_local SqlQuery s(wksp::cache(), "SELECT atom FROM ATags INNER JOIN Tags ON ATags.tag=Tags.id "
                            "WHERE tag=? ORDER BY Tags.k");
            auto s_af = s.af();
            s.bind(0, t.id);
            if(s.exec()){
                while(s.next())
                    ret << Atom{s.valueU64(0)};
            }
            return ret;
        }

        Vector<Atom>    all_atoms_unsorted(Tag t)
        {
            Vector<Atom>    ret;
            static thread_local SqlQuery s(wksp::cache(), "SELECT atom FROM ATags WHERE tag=?");
            auto s_af = s.af();
            s.bind(0, t.id);
            if(s.exec()){
                while(s.next())
                    ret << Atom{s.valueU64(0)};
            }
            return ret;
        }
    }
    
    Vector<Atom>        all_atoms(Tag t,Sorted sorted)
    {
        return sorted ? all_atoms_sorted(t) : all_atoms_unsorted(t);
    }
    
    size_t              all_atoms_count()
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT COUNT(1) FROM Atoms");
        auto s_af = s.af();
        if(s.exec() && s.next())
            return (size_t) s.valueU64(0);
        return 0;
    }
    
    size_t              all_atoms_count(Class c)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT COUNT(1) FROM AClasses WHERE class=?");
        auto s_af = s.af();
        s.bind(0, c.id);
        if(s.exec() && s.next())
            return (size_t) s.valueU64(0);
        return 0;
    }
    
    size_t              all_atoms_count(Tag t)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT COUNT(1) FROM ATags WHERE tag=?");
        auto s_af = s.af();
        s.bind(0, t.id);
        if(s.exec() && s.next())
            return (size_t) s.valueU64(0);
        return 0;
    }
    
    
    Atom                atom(uint64_t i)
    {
        return exists_atom(i) ? Atom{i} : Atom();
    }
    
    Atom                atom(const QString& k)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT id FROM Atoms WHERE k=? LIMIT 1");
        auto s_af = s.af();
        s.bind(0, k);
        if(s.exec() && s.next())
            return Atom{s.valueU64(0)};
        return Atom();
    }
    
    namespace {
        //Vector<Atom>        atoms_sorted(Atom);
        
        Vector<Atom>        atoms_unsorted(Atom a)
        {
            Vector<Atom> ret;
            static thread_local SqlQuery s(wksp::cache(), "SELECT  id FROM Atoms WHERE parent=?");
            auto s_af = s.af();
            s.bind(0, a.id);
            if(s.exec()){
                while(s.next())
                    ret << Atom{s.valueU64(0)};
            }
            return ret;
        }
        
    }

    Vector<Atom>            atoms(Atom a, Sorted sorted)
    {
        //  TODO (INNER JOIN)
        return atoms_unsorted(a);
    }

    namespace {
        #if 0
        Vector<Atom>    atoms_sorted(Document doc)
        {
            Vector<Atom>    ret;
            static thread_local SqlQuery s(wksp::cache(), "SELECT atom FROM ADocuments WHERE doc=?");
            auto s_af = s.af();
            s.bind(0, doc.id);
            if(s.exec()){
                while(s.next())
                    ret << Atom{s.valueU64(0)};
            }
            return ret;
        }
        #endif

        Vector<Atom>    atoms_unsorted(Document doc)
        {
            Vector<Atom>    ret;
            static thread_local SqlQuery s(wksp::cache(), "SELECT atom FROM ADocuments WHERE doc=?");
            auto s_af = s.af();
            s.bind(0, doc.id);
            if(s.exec()){
                while(s.next())
                    ret << Atom{s.valueU64(0)};
            }
            return ret;
        }
    }
    
    Vector<Atom>        atoms(Document doc, Sorted )
    {
        //   TODO inner join (above)
        return atoms_unsorted(doc); // sorted ? atoms_sorted(doc) : atoms_unsorted(doc);
    }
    
    size_t              atoms_count(Document doc)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT COUNT(1) FROM Atoms WHERE doc=?");
        auto s_af = s.af();
        s.bind(0, doc.id);
        if(s.exec() && s.next())
            return (size_t) s.valueU64(0);
        return 0;
    }
    
    QString             brief(Atom a)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT brief FROM Atoms WHERE id=?");
        auto s_af = s.af();
        s.bind(0, a.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }
    
    
    namespace {
        //Vector<Class>       classes_sorted(Atom a)
        
        Vector<Class>       classes_unsorted(Atom a)
        {
            Vector<Class>   ret;
            static thread_local SqlQuery s(wksp::cache(), "SELECT class FROM AClasses WHERE atom=?");
            auto s_af = s.af();
            s.bind(0, a.id);
            if(s.exec()){
                while(s.next())
                    ret << Class{s.valueU64(0)};
            }
            return ret;
        }
        
    }
    
    
    Vector<Class>       classes(Atom a, Sorted sorted)
    {
        return classes_unsorted(a); // TODO: Sorted
    }
    
    
    size_t              classes_count(Atom a)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT COUNT(1) FROM AClasses WHERE atom=?");
        auto s_af = s.af();
        s.bind(0, a.id);
        if(s.exec() && s.next())
            return (size_t) s.valueU64(0);
        return 0;
    }    
    


    Atom                db_atom(Document d, bool* wasCreated)
    {
        return db_atom(d, QString(), wasCreated);
    }
    
    Atom                db_atom(Document d, const QString&ck, bool* wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;

        QString     dk = strip_extension(key(d));
        if(dk.isEmpty()){
            yError() << "Cannot create to an empty key!";
            return Atom();
        }

        if(!ck.isEmpty())
            dk      += "#" + ck;
        
        static thread_local SqlQuery    i(wksp::cache(), "INSERT OR FAIL INTO Atoms (k,doc) VALUES (?,?)");
        static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Atoms WHERE k=?");
        auto i_af   = i.af();
        auto s_af   = s.af();
        i.bind(0, dk);
        i.bind(1, d.id);
        
        if(i.exec(false)){
            if(wasCreated)
                *wasCreated = true;
            return Atom{i.lastInsertIdU64()};
        } else {
            s.bind(0, dk);
            if(s.exec() && s.next())
                return Atom{s.valueU64(0)};
            yError() << "Unable to get the atom from the database: " << dk;
            return Atom();
        }
    }
    
    //Document            document(Atom a)
    //{
        //static thread_local SqlQuery s(wksp::cache(), "SELECT doc FROM Atoms WHERE id=?");
        //auto s_af = s.af();
        //s.bind(0, a.id);
        //if(s.exec() && s.next())
            //return Document{s.valueU64(0)};
        //return Document();
    //}
    
    Vector<Document>        documents(Atom a)
    {
        Vector<Document>    ret;
        
        static thread_local SqlQuery s(wksp::cache(), "SELECT doc FROM ADocuments WHERE id=?");
        auto s_af = s.af();
        s.bind(0, a.id);
        if(s.exec()){
            while(s.next())
                ret << Document{s.valueU64(0)};
        }
        return ret;
    }
    
    size_t                  documents_count(Atom a)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT COUNT(1) FROM ADocuments WHERE id=?");
        auto s_af = s.af();
        s.bind(0, a.id);
        if(s.exec() && s.next())
            return s.valueU64(0);
        return 0;
    }

   bool                exists(Atom a)
    {
        return exists_atom(a.id);
    }
    
   
    bool                exists_atom(uint64_t i)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT 1 FROM Atoms WHERE id=? LIMIT 1");
        auto s_lk   = s.af();
        s.bind(0, i);
        if(s.exec() && s.next())
            return s.valueAs<bool>(0);
        return false;
    }

    
   
    Image               icon(Atom a) 
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT icon FROM Atoms WHERE id=? LIMIT 1");
        auto s_af   = s.af();
        s.bind(0,a.id);
        if(s.exec() && s.next())
            return Image{s.valueU64(0)};
        return Image();
    }
    
    //Vector<Atom>            inbound(Atom);

    Atom::Info          info(Atom a)
    {
        Atom::Info    ret;
        
        static thread_local SqlQuery s(wksp::cache(), "SELECT k, abbr, brief, name FROM Atoms WHERE id=?");
        auto s_af = s.af();
        s.bind(0, a.id);
        if(s.exec() && s.next()){
            ret.key         = s.valueString(0);
            ret.abbr        = s.valueString(1);
            //ret.doc         = Document(s.valueU64(2));
            ret.brief       = s.valueString(2);
            ret.name        = s.valueString(3);
        }
        return ret;
    }
    
    
    bool                is(Atom a, Class c)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT 1 FROM AClasses WHERE atom=? AND class=?");
        auto s_af       = s.af();
        s.bind(0, a.id);
        s.bind(1, c.id);
        return s.exec() && s.next();
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
    
 
    QString             key(Atom a)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT k FROM Atoms WHERE id=?");
        auto s_af = s.af();
        s.bind(0, a.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }
    
    QString             label(Atom a)
    {
        QString n   = name(a);
        if(n.isEmpty())
            n       = key(a);
        return n;
    }
    
    
    //Leaf                    leaf(Atom

    QString             name(Atom a)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT name FROM Atoms WHERE id=? LIMIT 1");
        auto s_af = s.af();
        s.bind(0, a.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }

    
    NKI                 nki(Atom a, bool autoKey)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT name,icon,k FROM Atoms WHERE id=?");
        auto s_af = s.af();
        s.bind(0, a.id);
        if(s.exec() && s.next()){
            NKI  ret;
            ret.name    = s.valueString(0);
            ret.icon    = Image{s.valueU64(1)};
            ret.key     = s.valueString(2);
            if(autoKey && ret.name.isEmpty())
                ret.name    = ret.key;
            return ret;
        }
        return NKI();
    }
    

    //Vector<Atom>            outbound(Atom);

    Atom                parent(Atom a)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT parent FROM Atoms WHERE id=?");
        auto s_af = s.af();
        s.bind(0, a.id);
        if(s.exec() && s.next())
            return Atom{s.valueU64(0)};
        return Atom{};
    }
    
    
    QList<QVariant>         qvar_list(const Set<Atom>&all)
    {
        QVariantList    ret;
        for(const Atom t : all)
            ret << (quint64) t.id;
        return ret;
    }
    


    namespace {
        // Vector<Tag>     tags_sorted(Atom a);     // TODO (INNER JOIN)
        Vector<Tag>     tags_unsorted(Atom a)
        {
            Vector<Tag>    ret;
            static thread_local SqlQuery s(wksp::cache(), "SELECT tag FROM ATags WHERE atom=?");
            auto s_af = s.af();
            s.bind(0, a.id);
            if(s.exec()){
                while(s.next())
                    ret << Tag{s.valueU64(0)};
            }
            return ret;
        }
    }
    
    
    Vector<Tag>         tags(Atom a, Sorted sorted)
    {
        return tags_unsorted(a);
        //  TODO sorted....
    }

    bool                tagged(Atom a, Tag t)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT 1 FROM ATags WHERE atom=? AND tag=?");
        auto s_af = s.af();
        s.bind(0, a.id);
        s.bind(1, t.id);
        return s.exec() && s.next();
    }
    
    size_t              tags_count(Atom a)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT COUNT(1) FROM ATags WHERE atom=?");
        auto s_af   = s.af();
        s.bind(0, a.id);
        if(s.exec() && s.next())
            return (size_t) s.valueU64(0);
        return 0;
    }

    QString             title(Atom a)
    {
        return label(a);
    }
}
