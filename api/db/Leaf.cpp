////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Leaf.hpp"

#include "util/Logging.hpp"
#include "AtomSys.hpp"
#include "FileSys.hpp"
#include "Image.hpp"
#include "util/SqlQuery.hpp"
#include "util/Utilities.hpp"
#include "NKI.hpp"
#include "Root.hpp"
#include "Workspace.hpp"

namespace cdb {


    namespace {
        Vector<Atom>    all_leaf_atoms_sorted()
        {
            Vector<Atom>   ret;
            static thread_local SqlQuery s(wksp::cache(), "SELECT id FROM Atoms WHERE isLeaf=1 ORDER BY k");
            auto s_af = s.af();
            if(s.exec()){
                while(s.next())
                    ret << Atom{s.valueU64(0)};
            }
            return ret;
        }
        
        Vector<Atom>    all_leaf_atoms_unsorted()
        {
            Vector<Atom>   ret;
            static thread_local SqlQuery s(wksp::cache(), "SELECT id FROM Atoms WHERE leaf!=0");
            auto s_af = s.af();
            if(s.exec()){
                while(s.next())
                    ret << Atom{s.valueU64(0)};
            }
            return ret;
        }
    }
    
    
    Vector<Atom>        all_leaf_atoms(Sorted sorted)
    {
        return sorted ? all_leaf_atoms_sorted() : all_leaf_atoms_unsorted();
    }
    
    size_t              all_leaf_atoms_count()
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT COUNT(1) FROM Atoms WHERE leaf!=0");
        auto s_af   = s.af();
        if(s.exec() && s.next())
            return (size_t) s.valueU64(0);
        return 0;
    }
    
    namespace {
        Vector<Leaf>    all_leafs_sorted()
        {
            Vector<Leaf>    ret;
            static thread_local SqlQuery s(wksp::cache(), "SELECT id FROM Leafs ORDER BY k");
            auto s_af = s.af();
            if(s.exec()){
                while(s.next())
                    ret << Leaf{s.valueU64(0)};
            }
            return ret;
        }

        Vector<Leaf>    all_leafs_unsorted()
        {
            Vector<Leaf>    ret;
            static thread_local SqlQuery s(wksp::cache(), "SELECT id FROM Leafs");
            auto s_af = s.af();
            if(s.exec()){
                while(s.next())
                    ret << Leaf{s.valueU64(0)};
            }
            return ret;
        }
    }
    
    Vector<Leaf>        all_leafs(Sorted sorted)
    {
        return sorted ? all_leafs_sorted() : all_leafs_unsorted();
    }
    
    size_t              all_leafs_count()
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT COUNT(1) FROM Leafs");
        auto s_af = s.af();
        if(s.exec() && s.next())
            return (size_t) s.valueU64(0);
        return 0;
    }
    

    
    Atom                atom(Leaf l)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT atom FROM Leafs WHERE id=? LIMIT 1");
        auto s_af = s.af();
        s.bind(0, l.id);
        if(s.exec() && s.next())
            return Atom{s.valueU64(0)};
        return Atom();
    }
    
   

    
    Leaf                db_leaf(Document doc, bool*wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;
        if(!doc.id){
            yError() << "Cannot create to a null document!";
            return Leaf();
        }
        
        QString     k   = strip_extension(key(doc));
        if(k.isEmpty()){
            yError() << "Cannot create to blank key!";
            return Leaf();
        }
        
        static thread_local SqlQuery i(wksp::cache(), "INSERT INTO Leafs (id,k) VALUES (?,?)");
        auto i_af   = i.af();
        i.bind(0,doc.id);
        i.bind(1, k);
        if(i.exec(false)){
            if(wasCreated)
                *wasCreated = true;
            return Leaf(doc.id);
        } else if(exists_leaf(doc.id)){
            return Leaf(doc.id);
        } else {
            yError() << "Unable to create or find the leaf: " << k;
            return Leaf();
        }
    }


    
    Document            document(Leaf l)
    {
        return exists(l) ?  Document{l.id} : Document{};
    }
    
    bool                exists(Leaf l)
    {
        return exists_leaf(l.id);
    }
    

    bool                exists_leaf(uint64_t i)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT 1 FROM Leafs WHERE id=? LIMIT 1");
        auto s_lk   = s.af();
        s.bind(0, i);
        if(s.exec() && s.next())
            return s.valueAs<bool>(0);
        return false;
    }

    Folder              folder(Leaf l)
    {
        return folder(document(l));
    }
    
    
    Image               icon(Leaf l)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT icon FROM Leafs WHERE id=? LIMIT 1");
        auto s_af   = s.af();
        s.bind(0,l.id);
        if(s.exec() && s.next())
            return Image{s.valueU64(0)};
        return Image();
    }
    

    Leaf::Info          info(Leaf l, bool autoKey)
    {
        Leaf::Info    ret;
        static thread_local SqlQuery s(wksp::cache(), "SELECT k, title FROM Leafs WHERE id=?");
        auto s_af = s.af();
        s.bind(0, l.id);
        if(s.exec() && s.next()){
            ret.doc     = Document(l.id);
            ret.key     = s.valueString(0);
            ret.title   = s.valueString(2);
            if(autoKey && ret.title.isEmpty())
                ret.title   = ret.key;
        }
        return ret;
    }

    //bool                    is_leaf(Atom);
    
    
    QString             key(Leaf l)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT k FROM Leafs WHERE id=?");
        auto s_af = s.af();
        s.bind(0, l.id);
        if(s.exec() && s.next())    
            return s.valueString(0);
        return QString();
    }

    QString             label(Leaf l)
    {
        QString     n = title(l);
        return n.isEmpty() ? key(l) : n;
    }
    
    
    Leaf                leaf(uint64_t i)
    {
        return exists_leaf(i) ? Leaf{i} : Leaf();
    }

    Leaf                leaf(const QString& k)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT id FROM Leafs WHERE k=?");
        auto s_af = s.af();
        s.bind(0, k);
        if(s.exec() && s.next())
            return Leaf(s.valueU64(0));
        return Leaf{};
    }

    Leaf                leaf(Atom a)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT leaf FROM Atoms WHERE id=?");
        auto s_af   = s.af();
        s.bind(0, a.id);
        if(s.exec() && s.next())
            return Leaf{ s.valueU64(0) };
        return Leaf();
    }
    
    
    Leaf                leaf(Document d)
    {
        return exists_leaf(d.id) ? Leaf{d.id} : Leaf{};
    }
    
    Leaf                leaf(Folder f)
    {
        return leaf(key(f));
    }


    LeafFile::Shared         leaf_doc(Fragment f, bool fAllowEmpty)
    {
        if(!f)
            return LeafFile::Shared();

        Vector<char>    ch   = chars(f);
        if(ch.empty())
            return fAllowEmpty ? std::make_shared<LeafFile>() : LeafFile::Shared();
        
        LeafFile::Shared     td  = std::make_shared<LeafFile>();
        if(!td->load(ch, path(f).toStdString())){
            yError() << "Unable to read " << path(f);
            return LeafFile::Shared();
        }
        td -> set_file(path(f));
        return td;
    }

    
    Leaf::Merge::Shared         merged(Leaf l, unsigned int opts)
    {
        Leaf::Merge::Shared     ret = std::make_shared<Leaf::Merge>();
        bool    fFragUpdate     = static_cast<bool>(opts & IsUpdate);
        //bool    fAttributes     = !static_cast<bool>(opts & IgnoreAttributes);
        bool    fContext        = !static_cast<bool>(opts & IgnoreContext);
            
        for(auto& i : reads(l)){
        
            if(fFragUpdate)
                update(i.first);
        
            const Root* rt  = root(i.first);
                
            if(fContext && !i.second->body.empty())
                ret->context[rt] = i.second->context();
            ret->fusion(*i.second);
        }
        return ret;
    }
    

    
    NKI                 nki(Leaf l, bool autoKey)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT title,icon,k FROM Leafs WHERE id=?");
        auto s_af = s.af();
        s.bind(0, l.id);
        if(s.exec() && s.next()){
            NKI  ret;
            ret.name    = s.valueString(0);
            ret.icon    = Image(s.valueU64(1)) ;
            ret.key     = s.valueString(2);
            if(autoKey && ret.name.isEmpty())
                ret.name    = ret.key;
            return ret;
        }
        return NKI{};
    }

    LeafFile::Shared         read(Leaf l, const Root* rt)
    {
        return leaf_doc(fragment(document(l), rt));
    }

    Vector<Leaf::FragDoc>     reads(Leaf l)
    {
        Vector<Leaf::FragDoc>  ret;
        for(Fragment f : fragments(document(l), DataRole::DB)){
            LeafFile::Shared p   = leaf_doc(f);
            if(p)
                ret << Leaf::FragDoc(f,p);
        }
        return ret;
    }

    Vector<Leaf::FragDoc>   reads(Leaf l, class Root*rt)
    {
        Vector<Leaf::FragDoc>  ret;
        for(Fragment f : fragments(document(l), rt)){
            LeafFile::Shared p   = leaf_doc(f);
            if(p)
                ret << Leaf::FragDoc(f,p);
        }
        return ret;
    }

    QString             title(Leaf l)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT title FROM Leafs WHERE id=?");
        auto s_af = s.af();
        s.bind(0, l.id);
        if(s.exec() && s.next())    
            return s.valueString(0);
        return QString();
    }

    LeafFile::Shared         write(Leaf l, const Root* rt)
    {
        Document    d   = document(l);
        if(!d)
            return LeafFile::Shared();
        Fragment    f   = fragment(d);
        if(f)
            return leaf_doc(f, true);
        Folder      fo  = folder(d);
        if((fo != cdb::top_folder()) && !exists(fo, rt))
            rt -> make_path(key(fo));
        LeafFile::Shared ptr  = std::make_shared<LeafFile>();
        ptr -> set_file( rt -> resolve(key(d)));
        return ptr;
    }

}
