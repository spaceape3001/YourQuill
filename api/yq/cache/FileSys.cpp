////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "FileSys.hpp"
#include "NKI.hpp"

#include "yq/Loggers.hpp"
#include "yq/io/FileUtils.hpp"
#include "yq/sql/SqlQuery.hpp"
#include "yq/wksp/Root.hpp"
#include "yq/wksp/Workspace.hpp"

#include <QSqlError>

namespace cdb {
    namespace {
        Vector<Directory>   all_directories_sorted()
        {
            Vector<Directory>    ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Directories ORDER BY path");
            auto s_lk   = s.af();
            if(s.exec()){
                while(s.next())
                    ret << Directory{s.valueU64(0)};
            }
            return ret;
        }
        
        Vector<Directory>   all_directories_unsorted()
        {
            Vector<Directory>    ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Directories");
            auto s_lk   = s.af();
            if(s.exec()){
                while(s.next())
                    ret << Directory{s.valueU64(0)};
            }
            return ret;
        }
    }
    

    Vector<Directory>   all_directories(Sorted sorted)
    {
        return sorted ? all_directories_sorted() : all_directories_unsorted();
    }
    
    namespace {
        Vector<Directory>   all_directories_sorted(const Root* rt)
        {
            Vector<Directory>    ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Directories WHERE root=? ORDER BY path");
            auto s_lk   = s.af();
            s.bind(0, rt->id());
            if(s.exec()){
                while(s.next())
                    ret << Directory{s.valueU64(0)};
            }
            return ret;
        }
        
        Vector<Directory>   all_directories_unsorted(const Root*rt)
        {
            Vector<Directory>    ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Directories WHERE root=?");
            auto s_lk   = s.af();
            s.bind(0, rt->id());
            if(s.exec()){
                while(s.next())
                    ret << Directory{s.valueU64(0)};
            }
            return ret;
        }
   }
    
    
    Vector<Directory>   all_directories(const Root*rt, Sorted sorted)
    {
        if(!rt)
            return Vector<Directory>();
        return sorted ? all_directories_sorted(rt) : all_directories_unsorted(rt);
    }
    
    size_t              all_directories_count()
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT COUNT(1) FROM Directories");
        auto s_lk = s.af();
        if(s.exec() && s.next())
            return (size_t) s.valueU64(0);
        return 0;
    }
    
    size_t              all_directories_count(const Root*rt)
    {
        if(!rt)
            return 0;

        static thread_local SqlQuery    s(wksp::cache(), "SELECT COUNT(1) FROM Directories WHERE root=?");
        auto s_lk = s.af();
        s.bind(0, rt->id());
        if(s.exec() && s.next())
            return s.valueU64(0);
        return 0ULL;
    }
    
    namespace {
        Vector<Document>    all_documents_sorted()
        {
            Vector<Document>    ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Documents ORDER BY k");
            auto s_af   = s.af();
            if(s.exec()){
                while(s.next())
                    ret << Document{s.valueU64(0)};
            }
            return ret;
        }
        
        Vector<Document>    all_documents_unsorted()
        {
            Vector<Document>    ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Documents");
            auto s_af   = s.af();
            if(s.exec()){
                while(s.next())
                    ret << Document{s.valueU64(0)};
            }
            return ret;
        }
    }
    
    
    Vector<Document>    all_documents(Sorted sorted)
    {
        return sorted ? all_documents_sorted() : all_documents_unsorted();
    }
    
    size_t              all_documents_count()
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT COUNT(1) FROM Documents");
        auto s_af   = s.af();
        if(s.exec() && s.next())
            return (size_t) s.valueU64(0);
        return 0;
    }
    
    namespace {
        Vector<Document>    all_documents_suffix_sorted(const QString&sfx)
        {
            Vector<Document>    ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Documents WHERE suffix=? ORDER BY k");
            auto s_af   = s.af();
            s.bind(0, sfx);
            if(s.exec()){
                while(s.next())
                    ret << Document{s.valueU64(0)};
            }
            return ret;
        }

        Vector<Document>    all_documents_suffix_unsorted(const QString&sfx)
        {
            Vector<Document>    ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Documents WHERE suffix=?");
            auto s_af   = s.af();
            s.bind(0, sfx);
            if(s.exec()){
                while(s.next())
                    ret << Document{s.valueU64(0)};
            }
            return ret;
        }
    }
    
    Vector<Document>    all_documents_suffix(const QString&sfx, Sorted sorted)
    {
        return sorted ? all_documents_suffix_sorted(sfx) : all_documents_suffix_unsorted(sfx);
    }
    
    
   namespace {
        Vector<Folder>  all_folders_sorted()
        {
            Vector<Folder>  ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Folders ORDER BY k");
            auto s_af   = s.af();
            if(s.exec()){
                while(s.next())
                    ret << Folder{s.valueU64(0)};
            }
            return ret;
        }

        Vector<Folder>  all_folders_unsorted()
        {
            Vector<Folder>  ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Folders");
            auto s_af   = s.af();
            if(s.exec()){
                while(s.next())
                    ret << Folder{s.valueU64(0)};
            }
            return ret;
        }
    }

    Vector<Folder>      all_folders(Sorted sorted)
    {
        return sorted ? all_folders_sorted() : all_folders_unsorted();
    }
    
    size_t              all_folders_count()
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT COUNT(id) FROM Folders");
        auto s_af   =  s.af();
        if(s.exec() && s.next())
            return (size_t) s.valueU64(0);
        return 0;
    }
    
    namespace {
        Vector<Fragment>    all_fragments_sorted()
        {
            Vector<Fragment>    ret;
            static thread_local SqlQuery s(wksp::cache(), "SELECT id FROM Fragments ORDER BY path");
            auto s_af   = s.af();
            if(s.exec()){
                while(s.next())
                    ret << Fragment{s.valueU64(0)};
            }
            return ret;
        }
        
        Vector<Fragment>    all_fragments_unsorted()
        {
            Vector<Fragment>    ret;
            static thread_local SqlQuery s(wksp::cache(), "SELECT id FROM Fragments");
            auto s_af   = s.af();
            if(s.exec()){
                while(s.next())
                    ret << Fragment{s.valueU64(0)};
            }
            return ret;
        }
    }
    

    Vector<Fragment>    all_fragments(Sorted sorted)
    {
        return sorted ? all_fragments_sorted() : all_fragments_unsorted();
    }
    
    namespace {
        Vector<Fragment>    all_fragments_sorted(const Root* rt)
        {
            Vector<Fragment>    ret;
            if(rt){
                static thread_local SqlQuery s(wksp::cache(), "SELECT id FROM Fragments WHERE root=? ORDER BY path");
                auto s_af   = s.af();
                s.bind(0, rt->id());
                if(s.exec()){
                    while(s.next())
                        ret << Fragment{s.valueU64(0)};
                }
            }
            return ret;
        }

        Vector<Fragment>    all_fragments_unsorted(const Root* rt)
        {
            Vector<Fragment>    ret;
            if(rt){
                static thread_local SqlQuery s(wksp::cache(), "SELECT id FROM Fragments WHERE root=?");
                auto s_af   = s.af();
                s.bind(0, rt->id());
                if(s.exec()){
                    while(s.next())
                        ret << Fragment{s.valueU64(0)};
                }
            }
            return ret;
        }
    }
    
    Vector<Fragment>    all_fragments(const Root*rt, Sorted sorted)
    {
        if(!rt)
            return Vector<Fragment>();
        return sorted ? all_fragments_sorted(rt) : all_fragments_unsorted(rt);
    }
    
    size_t              all_fragments_count()
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT COUNT(1) FROM Fragments");
        auto s_af   = s.af();
        if(s.exec() && s.next())
            return (size_t) s.valueU64(0);
        return 0;
    }
    
    size_t              all_fragments_count(const Root*rt)
    {
        if(!rt)
            return 0;
        static thread_local SqlQuery s(wksp::cache(), "SELECT COUNT(1) FROM Fragments WHERE root=?");
        auto s_af   = s.af();
        s.bind(0, rt->id());
        if(s.exec() && s.next())
            return s.valueU64(0);
        return 0;
    }
    
    QString             base_key(const QString& key)
    {
        int i   = key.lastIndexOf('/');
        if(i<=0){
            int j   = key.indexOf('.');
            if(j<0)
                return key;
            return key.mid(0,j);
        } else {
            int j   = key.indexOf('.', i);
            if(j<0)
                return key.mid(i+1);
            return key.mid(i+1,j-i-1);
        }
    }

    QString             base_key(Document doc)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT base FROM Documents WHERE id=?");
        auto s_af       = s.af();
        s.bind(0, doc.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }
    
    QString             base_key(Fragment f)
    {
        return base_key(document(f));
    }

    QString             brief(Folder f)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT brief FROM Folders WHERE id=?");
        auto s_af = s.af();
        s.bind(0, f.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }
    
    QByteArray          bytes(Fragment f)
    {
        return file_bytes(path(f));
    }
    
    QByteArray          bytes(const QString&k)
    {
        return bytes(fragment(document(k)));
    }
    
    QByteArray          bytes(const QString&k, DataRole dr)
    {
        return bytes(fragment(document(k),dr));
    }
    
    
    Vector<char>        chars(Fragment f)
    {
        return file_load_char(path(f));
    }
    
    Vector<char>        chars(const QString&k)
    {
        return chars(fragment(document(k)));
    }
    
    Vector<char>        chars(const QString&k, DataRole dr)
    {
        return chars(fragment(document(k),dr));
    }
    
    DirOrFrag           child(Directory d, const QString& ck)
    {
        Fragment    f   = fragment(d, ck);
        if(f)
            return f;
        Directory   dir = directory(d, ck);
        if(dir)
            return dir;
        return false;
    }
    
    DocOrFold           child(Folder f, const QString& ck)
    {
        Document    doc   = document(f, ck);
        if(doc)
            return doc;
        Folder      fold  = folder(f, ck);
        if(fold)
            return fold;
        return false;
    }


    size_t              child_count(Directory d)
    {
        return directories_count(d) + fragments_count(d);
    }
    
    size_t              child_count(Folder f)
    {
        return documents_count(f) + folders_count(f);
    }


#if 0
    QString                 child_key(Directory);       //<! Key inside the directory (with extensions)
    QString                 child_key(Document);
    QString                 child_key(Folder);
    QString                 child_key(Fragment);
#endif


    Vector<DirOrFrag>   children(Directory d, Sorted sorted)
    {
        Vector<DirOrFrag>   ret;
        for(Directory d2 : directories(d, sorted))
            ret << d2;
        for(Fragment f : fragments(d, sorted))
            ret << f;
        return ret;
    }
    
    Vector<DocOrFold>   children(Folder, Sorted sorted);           // TODO
    Vector<DirOrFrag>   children(const Root* rt, Sorted sorted)
    {
        return children(directory(rt), sorted);
    }

    Vector<DirOrFragStr>    children_and_names(Directory);  // TODO
    Vector<DocOrFoldStr>    children_and_names(Folder);     // TODO
    
    
    Vector<uint8_t>         data(Fragment);     // TODO

    Directory           db_directory(const Root*rt, bool *wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;
        if(!rt)
            return Directory();
            
        static thread_local SqlQuery    i(wksp::cache(), "INSERT OR FAIL INTO Directories (path,root,folder,parent) VALUES (?,?,1,0)");
        static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Directories WHERE path=?");
        auto s_lk   = s.af();
        auto i_lk   = i.af();
        i.bind(0, rt->path());
        i.bind(1, rt->id());
        if(i.exec(false)){
            if(wasCreated)
                *wasCreated  = true;
            return Directory(i.lastInsertIdU64());
        } else {
            s.bind(0, rt->path());
            if(s.exec() && s.next())
                return Directory(s.valueU64(0));
            yError() << "Sql Query error!" << s.lastError() << "  Unable to get directory ID";
            return Directory();
        }
    }
    
    Directory           db_directory(Directory dir, const QString& k, bool *wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;
        if(k.isEmpty() || !dir)
            return Directory();
        Folder  f       = db_folder(folder(dir), k);
        QString p       = path(dir) + "/" + k;
        
        const Root* rt  = root(dir);
        if(!f || !rt)
            return Directory();
            
        static thread_local SqlQuery    i(wksp::cache(), "INSERT OR FAIL INTO Directories (path,root,folder,parent,name) VALUES (?,?,?,?,?)");
        static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Directories WHERE path=?");
        auto s_lk   = s.af();
        auto i_lk   = i.af();

        i.bind(0, p);
        i.bind(1,rt->id());
        i.bind(2,f.id);
        i.bind(3,dir.id);
        i.bind(4,k);
        if(i.exec(false)){
            if(wasCreated)
                *wasCreated = true;
            return Directory(i.lastInsertIdU64());
        } else {
            s.bind(0, p);
            if(s.exec() && s.next())
                return Directory(s.valueU64(0));
            yError() << "Sql Query error!" << s.lastError() << "  Unable to get directory ID";
            return Directory{};
        }
    }
    

    Document            db_document(Folder f, const QString&ak, bool *wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;
        if(!f)
            return Document();
        if(ak.isEmpty())
            return Document();

        QString     k   = key(f);
        if(k.isEmpty()){
            k   = ak;
        } else 
            k   = k + '/' + ak;
                
        int         x       = ak.lastIndexOf('.');
        int         y       = ak.indexOf('.',1);    // first period past any "hidden"
        QString     sfx     = (x > 0) ? ak.mid(x+1) : QString();
        QString     base    = (y > 0) ? ak.mid(0,y) : ak;
            
        static thread_local SqlQuery    i(wksp::cache(), "INSERT OR FAIL INTO Documents (k,sk,name,folder,suffix,base) VALUES (?,?,?,?,?,?)");
        static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Documents WHERE k=?");
        
        auto s_lk   = s.af();
        auto i_lk   = i.af();

        i.bind(0,k);
        i.bind(1,ak);
        i.bind(2,ak);
        i.bind(3,f.id);
        i.bind(4,sfx);
        i.bind(5,base);
        
        if(i.exec(false)){
            if(wasCreated)
                *wasCreated = true;
            return Document{i.lastInsertIdU64()};
        } else {
            s.bind(0,k);
            if(s.exec() && s.next())
                return Document{s.valueU64(0)};
            yError() << "Sql Query error!" << s.lastError() << "  Unable to get document ID";
            return Document();
        }

    }
    
    Folder              db_folder(Folder f, const QString&ck, bool *wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;
        if(!f)
            return Folder{};
        if(ck.isEmpty())
            return Folder{};

        QString     k = key(f);
        if(k.isEmpty()){
            k   = ck;
        } else 
            k   = k + '/' + ck;
            
        static thread_local SqlQuery    i(wksp::cache(), "INSERT OR FAIL INTO Folders (k,sk,name,parent,hidden) VALUES (?,?,?,?,?)");
        static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Folders WHERE k=?");

        auto s_lk   = s.af();
        auto i_lk   = i.af();
        i.bind(0,k);
        i.bind(1,ck);
        i.bind(2,ck);
        i.bind(3,f.id);
        i.bind(4,ck.contains('.'));
        if(i.exec(false)){
            if(wasCreated)
                *wasCreated = true;
            return Folder{i.lastInsertIdU64()};
        } else {
            s.bind(0,k);
            if(s.exec() && s.next())
                return Folder{s.valueU64(0)};
            yError() << "Sql Query error!" << s.lastError() << "  Unable to get folder ID";
            return Folder{};
        }    
    }
    
    
    Fragment            db_fragment(Directory dir, const QString& k, bool *wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;
        if(k.isEmpty() || !dir)
            return Fragment{};
        
        Folder          f   = folder(dir);
        Document        a   = db_document(f, k);
        QString         p   = path(dir) + "/" + k;
        const Root*     rt  = root(dir);
        
        
        static thread_local SqlQuery    i(wksp::cache(), "INSERT OR FAIL INTO Fragments (path,name,dir,root,document,folder) VALUES (?,?,?,?,?,?)");
        static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Fragments WHERE path=?");
        auto s_lk   = s.af();
        auto i_lk   = i.af();
        
        i.bind(0,p);
        i.bind(1,k);
        i.bind(2,dir.id);
        i.bind(3,rt->id());
        i.bind(4,a.id);
        i.bind(5,f.id);
        
        if(i.exec(false)){
            if(wasCreated)
                *wasCreated = true;
            return Fragment(i.lastInsertIdU64());
        } else {
            s.bind(0,p);
            if(s.exec() && s.next())
                return Fragment(s.valueU64(0));
            yError() << "Sql Query error!" << s.lastError() << "  Unable to get fragment ID";
            return Fragment{};
        }
    }
    
    Fragment                db_fragment(Document, const Root*); // TODO

    Directory           directory(uint64_t i)
    {
        return exists_directory(i) ? Directory{i} : Directory();
    }
    
    Directory           directory(const QString&path)
    {
        if(path.isEmpty())
            return Directory();
        static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Directories WHERE path=? LIMIT 1");
        auto s_lk   = s.af();
        s.bind(0, path);
        if(s.exec() && s.next())
            return Directory(s.valueU64(0));
        return Directory{};
    }
    
    Directory           directory(Fragment f)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT dir FROM Fragments WHERE id=?");
        auto s_af   = s.af();
        s.bind(0, f.id);
        if(s.exec() && s.next())
            return Directory(s.valueU64(0));
        return Directory{};
    }
    
    Directory           directory(Directory d, const QString&k)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT id FROM Directories WHERE parent=? AND name=?");
        auto s2_af  = s.af();
        s.bind(0, d.id);
        s.bind(1, k);
        if(s.exec() && s.next())
            return Directory{s.valueU64(0)};
        return Directory();
    }

    Directory           directory(const Root*rt)
    {
        if(!rt)
            return Directory();
        return directory(rt->path());
    }
    
    
    namespace {
        Vector<Directory>   directories_sorted(Directory d)
        {
            Vector<Directory>   ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Directories WHERE parent=? ORDER BY name");
            auto s_lk   = s.af();
            s.bind(0, d.id);
            if(s.exec()){
                while(s.next())
                    ret << Directory{s.valueU64(0)};
            }
            return ret;
        }

        Vector<Directory>   directories_unsorted(Directory d)
        {
            Vector<Directory>   ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Directories WHERE parent=?");
            auto s_lk   = s.af();
            s.bind(0, d.id);
            if(s.exec()){
                while(s.next())
                    ret << Directory{s.valueU64(0)};
            }
            return ret;
        }
    }
    
    
    Vector<Directory>   directories(Directory d, Sorted sorted)
    {
        return sorted ? directories_sorted(d) : directories_unsorted(d);
    }
    
    namespace {
        Vector<Directory>   directories_sorted(Folder f)
        {
            Vector<Directory>           ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Directories WHERE folder=? ORDER BY name");
            auto s_lk   = s.af();
            s.bind(0, f.id);
            if(s.exec()){
                while(s.next())
                    ret << Directory(s.valueU64(0));
            }
            return ret;
        }
        
        Vector<Directory>   directories_unsorted(Folder f)
        {
            Vector<Directory>           ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Directories WHERE folder=?");
            auto s_lk   = s.af();
            s.bind(0, f.id);
            if(s.exec()){
                while(s.next())
                    ret << Directory(s.valueU64(0));
            }
            return ret;
        }
    }
    
    Vector<Directory>   directories(Folder f, Sorted sorted)
    {
        return sorted ? directories_sorted(f) : directories_unsorted(f);
    }
    
    Vector<Directory>   directories(const Root*rt)
    {
        if(!rt)
            return Vector<Directory>();
        Vector<Directory>   ret;
        static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Directories WHERE parent=0 AND root=?");
        auto s_af   = s.af();
        s.bind(0, rt->id());
        if(s.exec()){
            while(s.next())
                ret << Directory{s.valueU64(0)};
        }
        return ret;
    }

    size_t              directories_count(Directory d)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT COUNT(1) FROM Directories WHERE parent=?");
        auto s_lk   = s.af();
        s.bind(0, d.id);
        if(s.exec() && s.next())
            return (size_t) s.valueU64(0);
        return 0;
    }
    
    size_t              directories_count(Folder f)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT COUNT(1) FROM Directories WHERE folder=?");
        auto s_af = s.af();
        s.bind(0, f.id);
        if(s.exec() && s.next())
            return (size_t) s.valueU64(0);
        return 0U;
    }
    
    size_t              directories_count(const Root* rt)
    {
        if(!rt)
            return 0;
        static thread_local SqlQuery    s(wksp::cache(), "SELECT COUNT(1) FROM Directories WHERE parent=0 AND root=?");
        auto s_lk   = s.af();
        s.bind(0, rt->id());
        if(s.exec() && s.next())
            return s.valueU64(0);
        return 0;
    }

    namespace {
        Vector<DirString>   directories_with_names_sorted(Directory dir)
        {
            Vector<DirString>   ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id,name FROM Directories WHERE parent=?");
            auto s_lk   = s.af();
            s.bind(0,dir.id);
            if(s.exec()){
                while(s.next())
                    ret << DirString(Directory(s.valueU64(0)), s.valueString(1));
            }
            return ret;
        }
        
        Vector<DirString>   directories_with_names_unsorted(Directory dir)
        {
            Vector<DirString>   ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id,name FROM Directories WHERE parent=? ORDER BY path");
            auto s_lk   = s.af();
            s.bind(0,dir.id);
            if(s.exec()){
                while(s.next())
                    ret << DirString(Directory(s.valueU64(0)), s.valueString(1));
            }
            return ret;
        }
    }

    Vector<DirString>   directories_with_names(Directory dir, Sorted sorted)
    {
        return sorted ? directories_with_names_sorted(dir) : directories_with_names_unsorted(dir);
    }
    
    
    Document            document(Folder f, const QString& k)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT id FROM Documents WHERE folder=? AND sk=?");
        auto s_af  = s.af();
        s.bind(0, f.id);
        s.bind(1, k);
        if(s.exec() && s.next())
            return Document{s.valueU64(0)};
        return Document();
    }

    Document            document(Fragment f)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT document FROM Fragments WHERE id=?");
        auto s_af   = s.af();
        s.bind(0, f.id);
        if(s.exec() && s.next())
            return Document(s.valueU64(0));
        return Document{};
    }

    Document            document(const QString& k)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Documents WHERE k=? LIMIT 1");
        auto s_af   = s.af();
        s.bind(0,k);
        if(s.exec() && s.next())
            return Document{s.valueU64(0)};
        return Document();
    }
    
    Document            document(uint64_t i)
    {
        return exists_document(i) ? Document{i} : Document();
    }


    //Vector<Document>    documents(Directory);

    
    namespace {
        Vector<Document>    documents_sorted(Folder f)
        {
            Vector<Document>           ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Documents WHERE folder=? ORDER BY sk");
            auto s_lk   = s.af();
            s.bind(0, f.id);
            if(s.exec()){
                while(s.next())
                    ret << Document(s.valueU64(0));
            }
            return ret;
        }
        
        Vector<Document>    documents_unsorted(Folder f)
        {
            Vector<Document>           ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Documents WHERE folder=?");
            auto s_lk   = s.af();
            s.bind(0, f.id);
            if(s.exec()){
                while(s.next())
                    ret << Document(s.valueU64(0));
            }
            return ret;
        }
    }
    
    
    Vector<Document>    documents(Folder f, Sorted sorted)
    {
        return sorted ? documents_sorted(f) : documents_unsorted(f);
    }
    
    size_t              documents_count(Folder f)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT COUNT(1) FROM Documents WHERE folder=?");
        auto s_af = s.af();
        s.bind(0, f.id);
        if(s.exec() && s.next())
            return (size_t) s.valueU64(0);
        return 0;
    }
    
    namespace {
        Vector<Document>    documents_by_suffix_sorted(Folder f, const QString&sfx)
        {
            Vector<Document>    ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Documents WHERE folder=? AND suffix=? ORDER BY k");
            auto s_lk       = s.af();
            s.bind(0, f.id);
            s.bind(1, sfx);
            if(s.exec()){
                while(s.next())
                    ret << Document(s.valueU64(0));
            }
            return ret;
        }

        Vector<Document>    documents_by_suffix_unsorted(Folder f, const QString&sfx)
        {
            Vector<Document>    ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Documents WHERE folder=? AND suffix=?");
            auto s_lk       = s.af();
            s.bind(0, f.id);
            s.bind(1, sfx);
            if(s.exec()){
                while(s.next())
                    ret << Document(s.valueU64(0));
            }
            return ret;
        }
    }
    
    Vector<Document>    documents_by_suffix(Folder f, const QString& sfx, Sorted sorted)
    {
        return sorted ? documents_by_suffix_sorted(f, sfx) : documents_by_suffix_unsorted(f,sfx);
    }
    
    
    namespace {
        Vector<Document>    documents_by_suffix_excluding_sorted(Folder f, const QString&sfx)
        {
            Vector<Document>    ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Documents WHERE folder!=? AND suffix=? ORDER BY k");
            auto s_lk       = s.af();
            s.bind(0, f.id);
            s.bind(1, sfx);
            if(s.exec()){
                while(s.next())
                    ret << Document(s.valueU64(0));
            }
            return ret;
        }
        
        Vector<Document>    documents_by_suffix_excluding_unsorted(Folder f, const QString&sfx)
        {
            Vector<Document>    ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Documents WHERE folder!=? AND suffix=?");
            auto s_lk       = s.af();
            s.bind(0, f.id);
            s.bind(1, sfx);
            if(s.exec()){
                while(s.next())
                    ret << Document(s.valueU64(0));
            }
            return ret;
        }
    }
    
    Vector<Document>    documents_by_suffix_excluding(Folder f, const QString&sfx, Sorted sorted)
    {
        return sorted ? documents_by_suffix_excluding_sorted(f,sfx) : documents_by_suffix_excluding_unsorted(f,sfx);
    }

    bool                exists(Directory d)
    {
        return exists_directory(d.id);
    }
    
    bool                exists(Document d)
    {
        return exists_document(d.id);
    }

    bool                exists(Folder f)
    {
        return exists_folder(f.id);
    }

    bool                exists(Folder fo, const Root*rt)
    {
        if(!rt)
            return false;
            
        static thread_local SqlQuery s(wksp::cache(), "SELECT 1 FROM Directories WHERE folder=? AND root=? LIMIT 1");
        auto s_af = s.af();
        s.bind(0, fo.id);
        s.bind(1, rt->id());
        if(s.exec() && s.next())    
            return s.valueAs<bool>(0);
        return false;
    }
    
    bool                exists(Fragment f)
    {
        return exists_fragment(f.id);
    }
    
    
    bool                exists(const Root*rt, const char*z)
    {
        if(!rt)
            return false;
        if(!z)
            return false;
        return rt -> exists(z);
    }
    
    bool                exists(const Root*rt, const QByteArray&z)
    {
        if(!rt)
            return false;
        return rt -> exists(z);
    }
    
    bool                exists(const Root*rt, const QString&z)
    {
        if(!rt)
            return false;
        return rt -> exists(z);
    }
    
    bool                exists(const Root*rt, const std::string&z)
    {
        if(!rt)
            return false;
        return rt -> exists(z);
    }
    
    bool                exists_directory(uint64_t i)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT 1 FROM Directories WHERE id=? LIMIT 1");
        auto s_lk   = s.af();
        s.bind(0, i);
        if(s.exec() && s.next())
            return s.valueAs<bool>(0);
        return false;
    }

    bool                exists_document(uint64_t i)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT 1 FROM Documents WHERE id=? LIMIT 1");
        auto s_lk   = s.af();
        s.bind(0, i);
        if(s.exec() && s.next())
            return s.valueAs<bool>(0);
        return false;
    }

    bool                exists_folder(uint64_t i)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT 1 FROM Folders WHERE id=? LIMIT 1");
        auto s_lk   = s.af();
        s.bind(0, i);
        if(s.exec() && s.next())
            return s.valueAs<bool>(0);
        return false;
    }

    bool                exists_fragment(uint64_t i)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT 1 FROM Fragments WHERE id=? LIMIT 1");
        auto s_lk   = s.af();
        s.bind(0, i);
        if(s.exec() && s.next())
            return s.valueAs<bool>(0);
        return false;
    }

    Fragment            first(Document d)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT id FROM Fragments WHERE document=? ORDER BY root LIMIT 1");
        auto s_af = s.af();
        s.bind(0, d.id);
        if(s.exec() && s.next())
            return Fragment{s.valueU64(0)};
        return Fragment{};
    }
    
    Fragment            first(Document d, const Root*rt)
    {
        if(!rt)
            return Fragment{};
        static thread_local SqlQuery s(wksp::cache(), "SELECT id FROM Fragments WHERE document=? AND root=? LIMIT 1");
        auto s_af = s.af();
        s.bind(0, d.id);
        s.bind(1, rt->id());
        if(s.exec() && s.next())
            return Fragment{s.valueU64(0)};
        return Fragment{};
    }

    Fragment            first(Document d, DataRole dr)
    {
        for(const Root* rt : wksp::root_reads()[dr]){
            Fragment f = first(d,rt);
            if(f)
                return f;
        }
        return Fragment{};
    }

    Folder              folder(Directory d)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT folder FROM Directories WHERE id=?");
        auto s_lk   = s.af();
        s.bind(0, d.id);
        if(s.exec() && s.next())
            return Folder(s.valueU64(0));
        return Folder{};
    }
    
    Folder              folder(Document d)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT folder FROM Documents WHERE id=? LIMIT 1");
        auto s_af       = s.af();
        s.bind(0, d.id);
        if(s.exec() && s.next())
            return Folder{s.valueU64(0)};
        return Folder{};
    }
    
    Folder              folder(Fragment f)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT folder FROM Fragments WHERE id=?");
        auto s_af   = s.af();
        s.bind(0, f.id);
        if(s.exec() && s.next())
            return Folder(s.valueU64(0));
        return Folder{};
    }
    
    Folder              folder(Folder f, const QString&ck)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Folders WHERE parent=? AND ck=? LIMIT 1");
        auto s_af = s.af();
        s.bind(0, f.id);
        s.bind(1, ck);
        if(s.exec() && s.next())
            return Folder{s.valueU64(0)};
        return Folder{};
    }
    

    Folder              folder(uint64_t i)
    {
        return exists_folder(i) ? Folder{i} : Folder{};
    }
    
    Folder              folder(const QString&k)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Folders WHERE k=? LIMIT 1");
        auto s_af   = s.af();
        
        s.bind(0,k);
        if(s.exec() && s.next())
            return Folder{s.valueU64(0)};
        return Folder{};
    }

    namespace {
        Vector<Folder>  folders_sorted(Folder f)
        {
            Vector<Folder>      ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Folders WHERE parent=? ORDER BY sk");
            auto s_af = s.af();
            s.bind(0, f.id);
            if(s.exec()){
                while(s.next())
                    ret << Folder{s.valueU64(0)};
            }
            return ret;
        }
        
        Vector<Folder>  folders_unsorted(Folder f)
        {
            Vector<Folder>      ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Folders WHERE parent=?");
            auto s_af = s.af();
            s.bind(0, f.id);
            if(s.exec()){
                while(s.next())
                    ret << Folder{s.valueU64(0)};
            }
            return ret;
        }
    }
    
    Vector<Folder>      folders(Folder f, Sorted sorted)
    {
        return sorted ? folders_sorted(f) : folders_unsorted(f);
    }
    
    
    size_t              folders_count(Folder f)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT COUNT(1) FROM Folders WHERE parent=?");
        auto s_af = s.af();
        s.bind(0, f.id);
        if(s.exec() && s.next())
            return (size_t) s.valueU64(0);
        return 0;
    }

    Fragment            fragment(const QString& k)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Fragments WHERE path=? LIMIT 1");
        auto s_af   = s.af();
        
        s.bind(0,k);
        if(s.exec() && s.next())
            return Fragment(s.valueU64(0));
        return Fragment{};
    }
    
    Fragment            fragment(uint64_t i)
    {
        return exists_fragment(i) ? Fragment{i} : Fragment{};
    }
    
    Fragment            fragment(const Root*rt, const char* z)
    {
        if(!rt)
            return Fragment{};
        return fragment(rt->resolve(z));
    }
    
    Fragment            fragment(const Root*rt, const QByteArray&z)
    {
        if(!rt)
            return Fragment{};
        return fragment(rt->resolve(z));
    }
    
    Fragment            fragment(const Root*rt, const QString&z)
    {
        if(!rt)
            return Fragment{};
        return fragment(rt->resolve(z));
    }
    
    Fragment            fragment(const Root*rt, const std::string&z)
    {
        if(!rt)
            return Fragment{};
        return fragment(rt->resolve(z));
    }
    
    Fragment            fragment(Directory d, const QString& k)
    {
        static thread_local SqlQuery s(wksp::cache(), "SELECT id FROM Fragments WHERE dir=? AND name=? LIMIT 1");
        auto s_af = s.af();
        s.bind(0, d.id);
        s.bind(1, k);
        if(s.exec() && s.next())
            return Fragment{s.valueU64(0)};
        return Fragment{};
    }
    
    Fragment            fragment(Document d)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Fragments WHERE document=? LIMIT 1");
        auto s_af   = s.af();
        s.bind(0, d.id);
        if(s.exec() && s.next())
            return Fragment(s.valueU64(0));
        return Fragment{};
    }
    
    Fragment            fragment(Document d, const Root* rt)
    {
        if(!rt)
            return Fragment{};
        static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Fragments WHERE document=? AND root=? LIMIT 1");
        auto s_af   = s.af();
        s.bind(0, d.id);
        s.bind(1,rt->id());
        if(s.exec() && s.next())
            return Fragment(s.valueU64(0));
        return Fragment{};
    }
    
    Fragment            fragment(Document d, DataRole dr)
    {
        for(const Root* r : wksp::root_reads(dr)){
            Fragment    f   = fragment(d, r);
            if(f)
                return f;
        }
        return Fragment();
    }


    namespace {
        Vector<Fragment>    fragments_sorted(Document d)
        {
            Vector<Fragment>    ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Fragments WHERE document=? ORDER BY path");
            auto s_af   = s.af();
            s.bind(0, d.id);
            if(s.exec()){
                while(s.next())
                    ret << Fragment{s.valueU64(0)};
            }
            return ret;
        }

        Vector<Fragment>    fragments_unsorted(Document d)
        {
            Vector<Fragment>    ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Fragments WHERE document=?");
            auto s_af   = s.af();
            s.bind(0, d.id);
            if(s.exec()){
                while(s.next())
                    ret << Fragment{s.valueU64(0)};
            }
            return ret;
        }
    }

    Vector<Fragment>    fragments(Document d, Sorted sorted)
    {
        return sorted ? fragments_sorted(d) : fragments_unsorted(d);
    }
    
    Vector<Fragment>        fragments(Document d, Sorted::Value sorted)
    {
        return sorted ? fragments_sorted(d) : fragments_unsorted(d);
    }
    
    namespace {
        Vector<Fragment>    fragments_sorted(Document d, const Root* rt)
        {
            if(!rt)
                return Vector<Fragment>();

            Vector<Fragment>    ret;
                
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Fragments WHERE document=? AND root=? ORDER BY path");
            auto s_af   = s.af();
            s.bind(0, d.id);
            s.bind(1, rt->id());
            if(s.exec()){
                while(s.next())
                    ret << Fragment{s.valueU64(0)};
            }
            return ret;
        }

        Vector<Fragment>    fragments_unsorted(Document d, const Root* rt)
        {
            if(!rt)
                return Vector<Fragment>();

            Vector<Fragment>    ret;
                
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Fragments WHERE document=? AND root=?");
            auto s_af   = s.af();
            s.bind(0, d.id);
            s.bind(1, rt->id());
            if(s.exec()){
                while(s.next())
                    ret << Fragment{s.valueU64(0)};
            }
            return ret;
        }
    }
    
    Vector<Fragment>    fragments(Document d, const Root* rt, Sorted sorted)
    {
        if(!rt)
            return Vector<Fragment>();
        return sorted ? fragments_sorted(d,rt) : fragments_unsorted(d,rt);
    }
    
    Vector<Fragment>    fragments(Document d, const Root* rt, Sorted::Value sorted)
    {
        return fragments(d, rt, Sorted{sorted});
    }

    Vector<Fragment>    fragments(Document d, DataRole dr, Sorted sorted)
    {
        Vector<Fragment>    ret;
        for(const Root* rt : wksp::root_reads()[dr])
            ret += fragments(d,rt,sorted);
        return ret;
    }

    namespace {
        Vector<Fragment>    fragments_sorted(Directory d)
        {
            Vector<Fragment>    ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Fragments WHERE dir=? ORDER BY path");
            auto s_lk = s.af();
            s.bind(0, d.id);
            if(s.exec()){
                while(s.next())
                    ret << Fragment(s.valueU64(0));
            } 
            return ret;
        }

        Vector<Fragment>    fragments_unsorted(Directory d)
        {
            Vector<Fragment>    ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Fragments WHERE dir=?");
            auto s_lk = s.af();
            s.bind(0, d.id);
            if(s.exec()){
                while(s.next())
                    ret << Fragment(s.valueU64(0));
            } 
            return ret;
        }
    }
    
    Vector<Fragment>    fragments(Directory d, Sorted sorted)
    {
        return sorted ? fragments_sorted(d) : fragments_unsorted(d);
    }
    
    namespace {
        Vector<Fragment>    fragments_sorted(Folder f)
        {
            Vector<Fragment>    ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Fragments WHERE folder=? ORDER BY path");
            auto s_lk = s.af();
            s.bind(0, f.id);
            if(s.exec()){
                while(s.next())
                    ret << Fragment(s.valueU64(0));
            } 
            return ret;
        }

        Vector<Fragment>    fragments_unsorted(Folder f)
        {
            Vector<Fragment>    ret;
            static thread_local SqlQuery    s(wksp::cache(), "SELECT id FROM Fragments WHERE folder=?");
            auto s_lk = s.af();
            s.bind(0, f.id);
            if(s.exec()){
                while(s.next())
                    ret << Fragment(s.valueU64(0));
            } 
            return ret;
        }
    }
    
    Vector<Fragment>    fragments(Folder f, Sorted sorted)
    {
        return sorted ? fragments_sorted(f) : fragments_unsorted(f);
    }
    
    Vector<Fragment>    fragments(const Root*rt, Sorted sorted)
    {
        return fragments(directory(rt), sorted);
    }
    
    Vector<Fragment>    fragments(const QString&k, Sorted sorted)
    {
        return fragments(document(k),sorted);
    }
    
    Vector<Fragment>    fragments(const QString&k, DataRole dr, Sorted sorted)
    {
        return fragments(document(k), dr, sorted);
    }

    size_t              fragments_count(Document d)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT COUNT(1) FROM Fragments WHERE document=?");
        auto s_af       = s.af();
        s.bind(0, d.id);
        if(s.exec() && s.next())
            return (size_t) s.valueU64(0);
        return 0;
    }
    
    size_t              fragments_count(Document d, const Root*rt)
    {
        if(!rt)
            return 0;
        static thread_local SqlQuery    s(wksp::cache(), "SELECT COUNT(1) FROM Fragments WHERE document=? AND root=?");
        auto s_af       = s.af();
        s.bind(0, d.id);
        s.bind(1, rt->id());
        if(s.exec() && s.next())
            return (size_t) s.valueU64(0);
        return 0;
    }
    
    size_t              fragments_count(Directory d)
    {
        static thread_local SqlQuery        s(wksp::cache(), "SELECT COUNT(1) FROM Fragments WHERE dir=?");
        auto s_af = s.af();
        s.bind(0, d.id);
        if(s.exec() && s.next())
            return (size_t) s.valueU64(0);
        return 0;
    }
    
    size_t              fragments_count(Folder f)
    {
        static thread_local SqlQuery        s(wksp::cache(), "SELECT COUNT(1) FROM Fragments WHERE folder=?");
        auto s_af = s.af();
        s.bind(0, f.id);
        if(s.exec() && s.next())
            return (size_t) s.valueU64(0);
        return 0;
    }

    bool                hidden(Directory d)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT hidden FROM Directories WHERE id=?");
        auto s_af   = s.af();
        s.bind(0,d.id);
        if(s.exec() && s.next())
            return s.valueAs<bool>(0);
        return false;
    }
    
    bool                hidden(Document d)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT hidden FROM Documents WHERE id=? LIMIT 1");
        auto s_af   = s.af();
        s.bind(0,d.id);
        if(s.exec() && s.next())
            return s.valueAs<bool>(0);
        return false;
    }
    
    bool                hidden(Folder f)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT hidden FROM Folders WHERE id=?");
        auto s_af   = s.af();
        s.bind(0,f.id);
        if(s.exec() && s.next())
            return s.valueAs<bool>(0);
        return false;
    }
    
    bool                hidden(Fragment f)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT hidden FROM Fragments WHERE id=?");
        auto s_af   = s.af();
        s.bind(0,f.id);
        if(s.exec() && s.next())
            return s.valueAs<bool>(0);
        return false;
    }
    
    void                hide(Document d)
    {
        static thread_local SqlQuery u(wksp::cache(), "UPDATE Documents SET hidden=1 WHERE id=?");
        auto u_af   = u.af();
        u.bind(0, d.id);
        u.exec();
    }
    
    Image               icon(Document d) 
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT icon FROM Documents WHERE id=? LIMIT 1");
        auto s_af   = s.af();
        s.bind(0,d.id);
        if(s.exec() && s.next())
            return Image{s.valueU64(0)};
        return Image();
    }

    Image               icon(Folder f)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT icon FROM Folders WHERE id=? LIMIT 1");
        auto s_af   = s.af();
        s.bind(0,f.id);
        if(s.exec() && s.next())
            return Image{s.valueU64(0)};
        return Image();
    }

    Directory::Info     info(Directory d)
    {
        Directory::Info        ret;
        static thread_local SqlQuery    s(wksp::cache(), "SELECT folder, name, parent, path, removed, root, hidden FROM Directories WHERE id=?");
        auto s_lk   = s.af();
        s.bind(0, d.id);
        if(s.exec() && s.next()){
            ret.folder  = Folder(s.valueU64(0));
            ret.name    = s.valueString(1);
            ret.parent  = Directory(s.valueU64(2));
            ret.path    = s.valueString(3);
            ret.removed = s.valueAs<bool>(4);
            ret.root    = wksp::root(s.valueU64(5));
            ret.hidden  = s.valueAs<bool>(6);
        }
        return ret;
    }
    
    Document::Info      info(Document d)
    {
        Document::Info        ret;
        static thread_local SqlQuery    s(wksp::cache(), "SELECT k, sk, name, base, folder, suffix, removed, hidden, icon FROM Documents WHERE id=?");
        auto s_af       = s.af();
        s.bind(0, d.id);
        if(s.exec() && s.next()){
            ret.key     = s.valueString(0);
            ret.skey    = s.valueString(1);
            ret.name    = s.valueString(2);
            ret.base    = s.valueString(3);
            ret.folder  = Folder(s.valueU64(4));
            ret.suffix  = s.valueString(5);
            ret.removed = s.valueAs<bool>(6);
            ret.hidden  = s.valueAs<bool>(7);
            ret.icon    = Image(s.valueU64(8));
        }
        return ret;
    }
    
    Folder::Info        info(Folder f)
    {
        Folder::Info        ret;
        static thread_local SqlQuery    s(wksp::cache(), "SELECT k, sk, parent, name, brief, hidden, removed, icon FROM Folders WHERE id=?");
        auto s_af   = s.af();
        s.bind(0,f.id);
        if(s.exec() && s.next()){
            ret.key         = s.valueString(0);
            ret.skey        = s.valueString(1);
            ret.parent      = Folder(s.valueU64(2));
            ret.name        = s.valueString(3);
            ret.brief       = s.valueString(4);
            ret.hidden      = s.valueAs<bool>(5);
            ret.removed     = s.valueAs<bool>(6);
            ret.icon        = Image(s.valueU64(7));
        }
        return ret;
    }
    
    Fragment::Info      info(Fragment f)
    {
        Fragment::Info        ret;

        static thread_local SqlQuery    s(wksp::cache(), "SELECT document, dir, folder, modified, name, path, removed, rescan, bytes, hidden, root FROM Fragments WHERE id=?");
        auto s_af   = s.af();
        s.bind(0, f.id);
        if(s.exec() && s.next()){
            ret.document        = Document(s.valueU64(0));
            ret.directory   = Directory(s.valueU64(1));
            ret.folder      = Folder(s.valueU64(2));
            ret.modified    = s.valueU64(3);
            ret.name        = s.valueString(4);
            ret.path        = s.valueString(5);
            ret.removed     = s.valueAs<bool>(6);
            ret.rescan      = s.valueAs<bool>(7);
            ret.size        = s.valueU64(8);
            ret.hidden      = s.valueAs<bool>(9);
            ret.root        = wksp::root(s.valueU64(10));
        }
        return ret;
    }

    QString             key(Directory d)
    {
        return key(folder(d));
    }
    
    QString             key(Document d)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT k FROM Documents WHERE id=? LIMIT 1");
        auto s_af       = s.af();
        s.bind(0, d.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }

    QString             key(Folder f) 
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT k FROM Folders WHERE id=?");
        auto s_af = s.af();
        s.bind(0, f.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }
    
    QString             key(Fragment f)
    {
        return key(document(f));
    }

    QString             key(const Root*rt)
    {
        if(!rt)
            return QString();
        return rt->key();
    }

    //QString             label(Directory);
    QString             label(Document d)
    {
        return key(d);
    }

    QString             label(Folder f)
    {
        return key(f);
    }
    
    QString             label(Fragment f)
    {
        return path(f);
    }
    

    uint64_t            modified(Fragment f)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT modified FROM Fragments WHERE id=?");
        auto s_af   = s.af();
        s.bind(0, f.id);
        if(s.exec() && s.next())
            return s.valueU64(0);
        return 0ULL;
    }

    QString             name(Directory d)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT name FROM Directories WHERE id=?");
        auto s_lk   = s.af();
        s.bind(0, d.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }
    
    QString             name(Document d)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT name FROM Documents WHERE id=? LIMIT 1");
        auto s_af       = s.af();
        s.bind(0, d.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }

    QString             name(Folder f)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT name FROM Folders WHERE id=?");
        auto s_af = s.af();
        s.bind(0, f.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }
    
    QString             name(Fragment f)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT name FROM Fragments WHERE id=?");
        auto s_af   = s.af();
        s.bind(0, f.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }

    NKI                 nki(Document d, bool autoKey)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT name,icon,k FROM Documents WHERE id=?");
        auto s_af = s.af();
        s.bind(0, d.id);
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

    NKI                 nki(Folder f, bool autoKey)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT name,icon,k FROM Folders WHERE id=?");
        auto s_af = s.af();
        s.bind(0, f.id);
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

    Directory           parent(Directory d)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT parent FROM Directories WHERE id=?");
        auto s_lk   = s.af();
        s.bind(0, d.id);
        if(s.exec() && s.next())
            return Directory(s.valueU64(0));
        return Directory();
    }
    
    Folder              parent(Document d)
    {
        return folder(d);
    }
    
    Folder              parent(Folder f)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT parent FROM Folders WHERE id=?");
        auto s_lk   = s.af();
        s.bind(0, f.id);
        if(s.exec() && s.next())
            return Folder(s.valueU64(0));
        return Folder();
    }
    
    Directory           parent(Fragment f)
    {
        return directory(f);
    }

    QString             path(Directory d)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT path FROM Directories WHERE id=?");
        auto s_lk   = s.af();
        s.bind(0, d.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }
    
    QString             path(Fragment f)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT path FROM Fragments WHERE id=?");
        auto s_af   = s.af();
        s.bind(0, f.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }
    
    QString             path(const Root*rt, const char*z, bool fMakePath)
    {
        QString p =  rt -> resolve(z);
        if(fMakePath){
            QFileInfo   fi(p);
            QDir().mkpath(fi.absolutePath());
        }
        return p;
    }
    
    QString             path(const Root*rt, const QByteArray&z, bool fMakePath)
    {
        QString p =  rt -> resolve(z);
        if(fMakePath){
            QFileInfo   fi(p);
            QDir().mkpath(fi.absolutePath());
        }
        return p;
    }
    
    QString             path(const Root*rt, const QString&z, bool fMakePath)
    {
        QString p =  rt -> resolve(z);
        if(fMakePath){
            QFileInfo   fi(p);
            QDir().mkpath(fi.absolutePath());
        }
        return p;
    }
    
    QString             path(const Root*rt, const std::string&z, bool fMakePath)
    {
        QString p =  rt -> resolve(z);
        if(fMakePath){
            QFileInfo   fi(p);
            QDir().mkpath(fi.absolutePath());
        }
        return p;
    }
    
    QString             path(const Root*rt, Document doc, bool fMakePath)
    {
        return path(rt, key(doc), fMakePath);
    }
    
    bool                removed(Directory d)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT removed FROM Directories WHERE id=?");
        auto s_af = s.af();
        s.bind(0, d.id);
        if(s.exec() && s.next())
            return s.valueAs<bool>(0);
        return false;
    }
    
    bool                removed(Document d)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT removed FROM Documents WHERE id=?");
        auto s_af       = s.af();
        s.bind(0, d.id);
        if(s.exec() && s.next())
            return s.valueAs<bool>(0);
        return false;
    }
    
    bool                removed(Folder f)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT removed FROM Folders WHERE id=?");
        auto s_af = s.af();
        s.bind(0, f.id);
        if(s.exec() && s.next())
            return s.valueAs<bool>(0);
        return false;
    }
    
    bool                removed(Fragment f)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT removed FROM Fragments WHERE id=?");
        auto s_af   = s.af();
        s.bind(0, f.id);
        if(s.exec() && s.next())
            return s.valueAs<bool>(0);
        return false;
    }
    
    void                rescan(Fragment f)
    {
        static thread_local SqlQuery    u(wksp::cache(), "UPDATE Fragments SET rescan=1 WHERE id=?");
        auto u_af   = u.af();
        u.bind(0, f.id);
        u.exec();
    }
    
    bool                rescanning(Fragment f)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT rescan FROM Fragments WHERE id=?");
        auto s_af = s.af();
        s.bind(0, f.id);
        if(s.exec() && s.next())
            return s.valueAs<bool>(0);
        return false;
    }    bool                    rescanning(Fragment);

    const Root*         root(Directory d)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT root FROM Directories WHERE id=?");
        auto s_lk   = s.af();
        s.bind(0, d.id);
        if(s.exec() && s.next())
            return wksp::root(s.valueU64(0));
        return nullptr;
    }
    
    const Root*         root(Fragment f)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT root FROM Fragments WHERE id=?");
        auto s_af   = s.af();
        s.bind(0, f.id);
        if(s.exec() && s.next())
            return wksp::root( s.valueU64(0));
        return nullptr;
    }

    Vector<const Root*> roots(Document d)
    {
        Vector<const Root*> ret;
        static thread_local SqlQuery    s(wksp::cache(), "SELECT DISTINCT root FROM Fragments WHERE document=?");
        auto s_af       = s.af();
        s.bind(0, d.id);
        if(s.exec()){
            while(s.next()){
                const Root*r    = wksp::root(s.valueU64(0));
                if(r)
                    ret << r;
            }
        }
        return ret;
    }
    
    Vector<const Root*> roots(Folder f)
    {
        Vector<const Root*> ret;
        static thread_local SqlQuery    s(wksp::cache(), "SELECT DISTINCT root FROM Directories WHERE folder=?");
        auto s_af       = s.af();
        s.bind(0, f.id);
        if(s.exec()){
            while(s.next()){
                const Root*r    = wksp::root(s.valueU64(0));
                if(r)
                    ret << r;
            }
        }
        return ret;
    }
    

    size_t              roots_count(Document d)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT COUNT(DISTINCT root) FROM Fragments WHERE document=?");
        auto s_af       = s.af();
        s.bind(0, d.id);
        if(s.exec() && s.next())
            return s.valueU64(0);
        return 0ULL;
    }
    
    size_t              roots_count(Folder f)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT COUNT(DISTINCT root) FROM Directories WHERE folder=?");
        auto s_af       = s.af();
        s.bind(0, f.id);
        if(s.exec() && s.next())
            return s.valueU64(0);
        return 0ULL;
    }
    
    void                show(Document d)
    {
        static thread_local SqlQuery u(wksp::cache(), "UPDATE Documents SET hidden=0 WHERE id=?");
        auto u_af   = u.af();
        u.bind(0, d.id);
        u.exec();
    }

    size_t              size(Fragment f)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT bytes FROM Fragments WHERE id=?");
        auto s_af   = s.af();
        s.bind(0, f.id);
        if(s.exec() && s.next())
            return s.valueU64(0);
        return 0ULL;
    }

    QString             skey(Directory d)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT name FROM Documents WHERE id=?");
        auto s_af       = s.af();
        s.bind(0, d.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }
    
    QString             skey(Document d)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT sk FROM Documents WHERE id=?");
        auto s_af       = s.af();
        s.bind(0, d.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }
    
    QString             skey(Folder f)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT sk FROM Folders WHERE id=?");
        auto s_af = s.af();
        s.bind(0, f.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }
    
    QString             skey(Fragment f)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT name FROM Fragments WHERE id=?");
        auto s_af   = s.af();
        s.bind(0, f.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }

    QString             skeyb(Directory d)
    {
        return base_key(skey(d));
    }
    
    QString             skeyb(Document d)
    {
        return base_key(skey(d));
    }
    
    QString             skeyb(Folder f)
    {
        return base_key(skey(f));
    }
    
    QString             skeyb(Fragment f)
    {
        return base_key(skey(f));
    }
    
    QString             suffix(Document d)
    {
        static thread_local SqlQuery    s(wksp::cache(), "SELECT suffix FROM Documents WHERE id=?");
        auto s_af       = s.af();
        s.bind(0, d.id);
        if(s.exec() && s.next())
            return s.valueString(0);
        return QString();
    }

    void                update(Fragment f)
    {
        String  p           = path(f);
        SizeTimestamp   sz  = file_size_and_timestamp(p.c_str());
    
        static thread_local SqlQuery    u(wksp::cache(), "UPDATE Fragments SET bytes=?,modified=?,removed=?,rescan=0 WHERE id=?");
        auto u_af = u.af();
        u.bind(0, sz.size);
        u.bind(1, sz.nanoseconds());
        u.bind(2, !sz.exists);
        u.bind(3, f.id);
        u.exec();  
    }

    Fragment            writable(Document d, DataRole dr)
    {
        for(const Root* rt : wksp::root_writes()[dr]){
            Fragment f = first(d, rt);
            if(f)
                return f;
        }
        return Fragment{};
    }
 }


