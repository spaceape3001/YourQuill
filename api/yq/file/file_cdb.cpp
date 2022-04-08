////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "cdb.hpp"
#include <yq/bit/NKI.hpp>
#include <yq/collection/vector_utils.hpp>
#include <yq/file/FileUtils.hpp>
#include <yq/sql/SqlLastId.hpp>
#include <yq/sql/SqlQuery.hpp>
#include <yq/text/Utils.hpp>
#include <yq/type/ByteArray.hpp>
#include <yq/wksp/CacheSQ.hpp>
#include <yq/wksp/Root.hpp>
#include <yq/wksp/Workspace.hpp>

namespace yq {
    namespace cdb {
        static constexpr const uint64_t     kMaxFixedFolder = Folder::TAGS;
    
        std::vector<Directory>   all_directories(unsigned opts)
        {
            if(opts & BestSort){
                if(opts & Hidden){
                    static thread_local SQ    s("SELECT id FROM Directories ORDER BY path");
                    return s.vec<Directory>();
                } else {
                    static thread_local SQ    s("SELECT id FROM Directories WHERE hidden=0 ORDER BY path");
                    return s.vec<Directory>();
                }
            } else {
                if(opts & Hidden){
                    static thread_local SQ    s("SELECT id FROM Directories");
                    return s.vec<Directory>();
                } else {
                    static thread_local SQ    s("SELECT id FROM Directories WHERE hidden=0");
                    return s.vec<Directory>();
                }
            }
        }
        
        std::vector<Directory>   all_directories(Sorted sorted)
        {
            return all_directories(Hidden | (sorted ? BestSort : 0));
        }

        namespace {
            std::vector<Directory>   all_directories_sorted(const Root* rt)
            {
                static thread_local SQ    s("SELECT id FROM Directories WHERE root=? ORDER BY path");
                return s.vec<Directory>(rt->id);
            }
            
            std::vector<Directory>   all_directories_unsorted(const Root*rt)
            {
                static thread_local SQ    s("SELECT id FROM Directories WHERE root=?");
                return s.vec<Directory>(rt->id);
            }
       }
        
        
        std::vector<Directory>   all_directories(const Root*rt, Sorted sorted)
        {
            if(!rt)
                return std::vector<Directory>();
            return sorted ? all_directories_sorted(rt) : all_directories_unsorted(rt);
        }
        
        size_t              all_directories_count()
        {
            static thread_local SQ    s("SELECT COUNT(1) FROM Directories");
            return s.size();
        }
        
        size_t              all_directories_count(const Root*rt)
        {
            if(!rt)
                return 0;

            static thread_local SQ    s("SELECT COUNT(1) FROM Directories WHERE root=?");
            return s.size(rt->id);
        }
        
        namespace {
            std::vector<Document>    all_documents_sorted()
            {
                static thread_local SQ    s("SELECT id FROM Documents ORDER BY k");
                return s.vec<Document>();
            }
            
            std::vector<Document>    all_documents_unsorted()
            {
                static thread_local SQ    s("SELECT id FROM Documents");
                return s.vec<Document>();
            }
        }
        
        
        std::vector<Document>    all_documents(Sorted sorted)
        {
            return sorted ? all_documents_sorted() : all_documents_unsorted();
        }
        
        size_t              all_documents_count()
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Documents");
            return s.size();
        }
        
        namespace {
            std::vector<Document>    all_documents_suffix_sorted(std::string_view sfx)
            {
                static thread_local SQ    s("SELECT id FROM Documents WHERE suffix=? ORDER BY k");
                return s.vec<Document>(sfx);
            }

            std::vector<Document>    all_documents_suffix_unsorted(std::string_view sfx)
            {
                static thread_local SQ    s("SELECT id FROM Documents WHERE suffix=?");
                return s.vec<Document>(sfx);
            }
        }
        
        std::vector<Document>    all_documents_suffix(std::string_view sfx, Sorted sorted)
        {
            return sorted ? all_documents_suffix_sorted(sfx) : all_documents_suffix_unsorted(sfx);
        }
        
        
       namespace {
            std::vector<Folder>  all_folders_sorted()
            {
                static thread_local SQ    s("SELECT id FROM Folders ORDER BY k");
                return s.vec<Folder>();
            }

            std::vector<Folder>  all_folders_unsorted()
            {
                static thread_local SQ    s("SELECT id FROM Folders");
                return s.vec<Folder>();
            }
        }

        std::vector<Folder>      all_folders(Sorted sorted)
        {
            return sorted ? all_folders_sorted() : all_folders_unsorted();
        }
        
        size_t              all_folders_count()
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Folders");
            return s.size();
        }
        
        namespace {
            std::vector<Fragment>    all_fragments_sorted()
            {
                static thread_local SQ s("SELECT id FROM Fragments ORDER BY path");
                return s.vec<Fragment>();
            }
            
            std::vector<Fragment>    all_fragments_unsorted()
            {
                static thread_local SQ s("SELECT id FROM Fragments");
                return s.vec<Fragment>();
            }
        }
        

        std::vector<Fragment>    all_fragments(Sorted sorted)
        {
            return sorted ? all_fragments_sorted() : all_fragments_unsorted();
        }
        
        namespace {
            std::vector<Fragment>    all_fragments_sorted(const Root* rt)
            {
                static thread_local SQ s("SELECT id FROM Fragments WHERE root=? ORDER BY path");
                return s.vec<Fragment>(rt->id);
            }

            std::vector<Fragment>    all_fragments_unsorted(const Root* rt)
            {
                static thread_local SQ s("SELECT id FROM Fragments WHERE root=?");
                return s.vec<Fragment>(rt->id);
            }
        }
        
        std::vector<Fragment>    all_fragments(const Root*rt, Sorted sorted)
        {
            if(!rt)
                return std::vector<Fragment>();
            return sorted ? all_fragments_sorted(rt) : all_fragments_unsorted(rt);
        }
        
        size_t              all_fragments_count()
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Fragments");
            return s.size();
        }
        
        size_t              all_fragments_count(const Root*rt)
        {
            if(!rt)
                return 0;
            static thread_local SQ s("SELECT COUNT(1) FROM Fragments WHERE root=?");
            return s.size(rt->id);
        }
        
        namespace {
            std::vector<Fragment>    all_fragments_suffix_sorted(std::string_view sfx)
            {
                static thread_local SQ s("SELECT id FROM Fragments WHERE suffix=? ORDER BY path");
                return s.vec<Fragment>(sfx);
            }
            
            std::vector<Fragment>    all_fragments_suffix_unsorted(std::string_view sfx)
            {
                static thread_local SQ s("SELECT id FROM Fragments WHERE suffix=?");
                return s.vec<Fragment>(sfx);
            }
        }

        std::vector<Fragment>    all_fragments_suffix(std::string_view sfx, Sorted sorted)
        {
            return sorted ? all_fragments_suffix_sorted(sfx) : all_fragments_suffix_unsorted(sfx);
        }

        std::string_view         base_key(std::string_view key)
        {
            size_t i   = key.find_last_of('/');
            if(i == std::string_view::npos){
                size_t j   = key.find_first_of('.');
                if(j == std::string_view::npos)
                    return key;
                return key.substr(0,j);
            } else {
                size_t j   = key.find_first_of('.', i);
                if(j == std::string_view::npos)
                    return key.substr(i+1);
                return key.substr(i+1,j-i-1);
            }
        }

        std::string             base_key(Document doc)
        {
            static thread_local SQ    s("SELECT base FROM Documents WHERE id=?");
            return s.str(doc.id);
        }
        
        std::string             base_key(Fragment f)
        {
            return base_key(document(f));
        }

        std::string             brief(Folder f)
        {
            static thread_local SQ    s("SELECT brief FROM Folders WHERE id=?");
            return s.str(f.id);
        }
        
        
        DirOrFrag           child(Directory d, std::string_view ck)
        {
            Fragment    f   = fragment(d, ck);
            if(f)
                return f;
            Directory   dir = directory(d, ck);
            if(dir)
                return dir;
            return false;
        }
        
        DocOrFold           child(Folder f, std::string_view ck)
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
        std::string                 child_key(Directory);       //<! Key inside the directory (with extensions)
        std::string                 child_key(Document);
        std::string                 child_key(Folder);
        std::string                 child_key(Fragment);
    #endif


        std::vector<DirOrFrag>   children(Directory d, Sorted sorted)
        {
            std::vector<DirOrFrag>   ret;
            for(Directory d2 : directories(d, sorted))
                ret.push_back(d2);
            for(Fragment f : fragments(d, sorted))
                ret.push_back(f);
            return ret;
        }
        
        std::vector<DocOrFold>   children(Folder, Sorted sorted);           // TODO
        std::vector<DirOrFrag>   children(const Root* rt, Sorted sorted)
        {
            return children(directory(rt), sorted);
        }

        std::vector<DirOrFragStr>    children_and_names(Directory);  // TODO
        std::vector<DocOrFoldStr>    children_and_names(Folder);     // TODO
        
        
        std::vector<uint8_t>         data(Fragment);     // TODO

        std::pair<Directory, Folder>    db_directory(Directory dirParent, std::string_view k, bool *wasCreated)
        {
            if(wasCreated)
                *wasCreated = false;
            if(k.empty() || !dirParent)
                return {};
            const Root* rt  = root(dirParent);
            if(!rt)
                return {};

            Folder f           = db_folder(folder(dirParent), k);
            if(!f)
                return {};
                
            std::string p       = path(dirParent);
            p += '/';
            p += k;
            
                
            static thread_local SQ    i("INSERT OR FAIL INTO Directories (path,root,folder,parent,name) VALUES (?,?,?,?,?)");
            static thread_local SQ    s("SELECT id FROM Directories WHERE path=?");
            auto s_lk   = s.af();
            auto i_lk   = i.af();

            i.bind(1, p);
            i.bind(2,rt->id);
            i.bind(3,f.id);
            i.bind(4,dirParent.id);
            i.bind(5,k);
            
            if(is_good(i.step())){
                if(wasCreated)
                    *wasCreated = true;
                return { Directory((uint64_t) i.last_id()), f};
            } else {
                s.bind(1, p);
                if(s.step() == SqlQuery::Row)
                    return { Directory(s.v_uint64(1)), f };
                cdbError << "Unable to get directory ID";
                return {};
            }
        }
        

        Document            db_document(Folder f, std::string_view ak, bool *wasCreated)
        {
            if(wasCreated)
                *wasCreated = false;
            if(!f)
                return Document();
            if(ak.empty() || is_space(ak))
                return Document();

            std::string     k   = key(f);
            if(k.empty()){
                k   = ak;
            } else {
                k   += '/';
                k   += ak;
            }
                    
            size_t  x = ak.find_last_of('.');
            size_t  y = ak.find_first_of('.',1);    // first period past any "hidden"
            std::string_view     sfx     = (x != std::string_view::npos ) ? ak.substr(x+1) : std::string_view();
            std::string_view     base    = (y != std::string_view::npos ) ? ak.substr(0,y) : ak;
                
            static thread_local SQ    i("INSERT OR FAIL INTO Documents (k,sk,name,folder,suffix,base,hidden) VALUES (?,?,?,?,?,?,?)");
            static thread_local SQ    s("SELECT id FROM Documents WHERE k=?");
            
            auto s_lk   = s.af();
            auto i_lk   = i.af();

            i.bind(1,k);
            i.bind(2,ak);
            i.bind(3,ak);
            i.bind(4,f.id);
            i.bind(5,sfx);
            i.bind(6,base);
            i.bind(7,ak[0] == '.');
            
            if(is_good(i.step())){
                if(wasCreated)
                    *wasCreated = true;
                return Document((uint64_t) i.last_id());
            } else {
                s.bind(1,k);
                if(s.step() == SqlQuery::Row)
                    return Document{s.v_uint64(1)};
                cdbError << "Unable to get documednt ID";
                return Document();
            }

        }
        
        Folder              db_folder(Folder f, std::string_view ck, bool *wasCreated)
        {
            if(wasCreated)
                *wasCreated = false;
            if(!f)
                return Folder{};
            if(ck.empty())
                return Folder{};

            std::string     k = key(f);
            if(k.empty()){
                k   = ck;
            } else {
                k += '/';
                k += ck;
            }
                
            static thread_local SQ    i("INSERT OR FAIL INTO Folders (k,sk,name,parent,hidden) VALUES (?,?,?,?,?)");
            static thread_local SQ    s("SELECT id FROM Folders WHERE k=?");

            auto s_lk   = s.af();
            auto i_lk   = i.af();
            
            
            bool    hidden  = (ck[0] == '.') || (ck.find_first_of('.') != std::string_view::npos);
            
            i.bind(1,k);
            i.bind(2,ck);
            i.bind(3,ck);
            i.bind(4,f.id);
            i.bind(5,hidden);
            
            if(is_good(i.step())){
                if(wasCreated)
                    *wasCreated = true;
                return Folder{(uint64_t) i.last_id()};
            } else {
                s.bind(1,k);
                if(s.step() == SqlQuery::Row)
                    return Folder{s.v_uint64(1)};
                cdbError << "Unable to get folder ID";
                return Folder{};
            }    
        }
        
        
        std::pair<Fragment, Document>   db_fragment(Directory dirParent, std::string_view k, bool *wasCreated)
        {
            if(wasCreated)
                *wasCreated = false;
            if(k.empty() || is_space(k) || !dirParent)
                return {};

            Folder          f   = folder(dirParent);
            Document        a   = db_document(f, k);
            std::filesystem::path         p   = path(dirParent) / k;
            std::string         sfx = suffix(a);
            const Root*     rt  = root(dirParent);
            
            
            static thread_local SQ    i("INSERT OR FAIL INTO Fragments (path,name,dir,root,document,folder,suffix) VALUES (?,?,?,?,?,?,?)");
            static thread_local SQ    s("SELECT id FROM Fragments WHERE path=?");
            auto s_lk   = s.af();
            auto i_lk   = i.af();
            
            i.bind(1,p);
            i.bind(2,k);
            i.bind(3,dirParent.id);
            i.bind(4,rt->id);
            i.bind(5,a.id);
            i.bind(6,f.id);
            i.bind(7,sfx);
            
            if(is_good(i.step())){
                if(wasCreated)
                    *wasCreated = true;
                return { Fragment((uint64_t) i.last_id()), a };
            } else {
                s.bind(1,p);
                if(s.step() == SqlQuery::Row)
                    return { Fragment(s.v_uint64(1)), a };
                cdbError << "Unable to get fragment ID";
                return {};
            }
        }
        
        Directory           db_root(const Root*rt, bool *wasCreated)
        {
            if(wasCreated)
                *wasCreated = false;
            if(!rt)
                return Directory();
                
            static thread_local SQ    i("INSERT OR FAIL INTO Directories (path,root,folder,parent) VALUES (?,?,1,0)");
            static thread_local SQ    s("SELECT id FROM Directories WHERE path=?");
            
            auto s_lk   = s.af();
            auto i_lk   = i.af();
            i.bind(1, rt->path);
            i.bind(2, rt->id);
            
            if(is_good(i.step())){
                if(wasCreated)
                    *wasCreated  = true;
                return Directory(i.last_id());
            } else {
                s.bind(1, rt->path);
                if(s.step() == SqlQuery::Row)
                    return Directory(s.v_uint64(1));
                cdbError << "Unable to get directory ID";
                return Directory();
            }
        }
        
        //Fragment                db_fragment(Document, const Root*); // TODO

        Directory           directory(uint64_t i)
        {
            return exists_directory(i) ? Directory{i} : Directory();
        }
        
        Directory           directory(std::string_view path)
        {
            if(path.empty())
                return Directory();
            static thread_local SQ    s("SELECT id FROM Directories WHERE path=? LIMIT 1");
            return s.as<Directory>(path);
        }

        Directory           directory(const std::filesystem::path&p)
        {
            if(p.empty())
                return Directory();
            static thread_local SQ    s("SELECT id FROM Directories WHERE path=? LIMIT 1");
            return s.as<Directory>(p);
        }
        
        Directory           directory(Fragment f)
        {
            static thread_local SQ    s("SELECT dir FROM Fragments WHERE id=?");
            return s.as<Directory>(f.id);
        }
        
        Directory           directory(Directory d, std::string_view k)
        {
            static thread_local SQ s("SELECT id FROM Directories WHERE parent=? AND name=?");
            return s.as<Directory>(d.id, k);
        }

        Directory           directory(const Root*rt)
        {
            if(!rt)
                return Directory();
            return directory(rt->path);
        }
        
        
        namespace {
            std::vector<Directory>   directories_sorted(Directory d)
            {
                static thread_local SQ    s("SELECT id FROM Directories WHERE parent=? ORDER BY name");
                return s.vec<Directory>(d.id);
            }

            std::vector<Directory>   directories_unsorted(Directory d)
            {
                static thread_local SQ    s("SELECT id FROM Directories WHERE parent=?");
                return s.vec<Directory>(d.id);
            }
        }
        
        
        std::vector<Directory>   directories(Directory d, Sorted sorted)
        {
            return sorted ? directories_sorted(d) : directories_unsorted(d);
        }
        
        namespace {
            std::vector<Directory>   directories_sorted(Folder f)
            {
                static thread_local SQ    s("SELECT id FROM Directories WHERE folder=? ORDER BY name");
                return s.vec<Directory>(f.id);
            }
            
            std::vector<Directory>   directories_unsorted(Folder f)
            {
                static thread_local SQ    s("SELECT id FROM Directories WHERE folder=?");
                return s.vec<Directory>(f.id);
            }
        }
        
        std::vector<Directory>   directories(Folder f, Sorted sorted)
        {
            return sorted ? directories_sorted(f) : directories_unsorted(f);
        }
        
        std::vector<Directory>   directories(const Root*rt)
        {
            if(!rt)
                return std::vector<Directory>();
                
            static thread_local SQ    s("SELECT id FROM Directories WHERE parent=0 AND root=?");
            return s.vec<Directory>(rt->id);
        }

        size_t              directories_count(Directory d)
        {
            static thread_local SQ    s("SELECT COUNT(1) FROM Directories WHERE parent=?");
            return s.size(d.id);
        }
        
        size_t              directories_count(Folder f)
        {
            static thread_local SQ    s("SELECT COUNT(1) FROM Directories WHERE folder=?");
            return s.size(f.id);
        }
        
        size_t              directories_count(const Root* rt)
        {
            if(!rt)
                return 0;
            static thread_local SQ    s("SELECT COUNT(1) FROM Directories WHERE parent=0 AND root=?");
            return s.size(rt->id);
        }

        std::set<Directory>             directories_set(Directory d)
        {
            static thread_local SQ    s("SELECT id FROM Directories WHERE parent=?");
            return s.set<Directory>(d.id);
        }
        

        namespace {
            std::vector<DirString>   directories_with_names_sorted(Directory dir)
            {
                std::vector<DirString>   ret;
                static thread_local SQ    s("SELECT id,name FROM Directories WHERE parent=?");
                s.bind(1,dir.id);
                while(s.step() == SqlQuery::Row){
                    ret.push_back(DirString(Directory(s.v_uint64(1)), s.v_text(1)));
                }
                s.reset();
                return ret;
            }
            
            std::vector<DirString>   directories_with_names_unsorted(Directory dir)
            {
                std::vector<DirString>   ret;
                static thread_local SQ    s("SELECT id,name FROM Directories WHERE parent=? ORDER BY path");
                s.bind(1,dir.id);
                while(s.step() == SqlQuery::Row){
                    ret.push_back(DirString(Directory(s.v_uint64(1)), s.v_text(1)));
                }
                s.reset();
                return ret;
            }
        }

        std::vector<DirString>   directories_with_names(Directory dir, Sorted sorted)
        {
            return sorted ? directories_with_names_sorted(dir) : directories_with_names_unsorted(dir);
        }
        
        
        Document            document(Folder f, std::string_view k)
        {
            static thread_local SQ s("SELECT id FROM Documents WHERE folder=? AND sk=?");
            return s.as<Document>(f.id, k);
        }

        Document            document(Fragment f)
        {
            static thread_local SQ    s("SELECT document FROM Fragments WHERE id=?");
            return s.as<Document>(f.id);
        }

        Document            document(std::string_view k)
        {
            static thread_local SQ    s("SELECT id FROM Documents WHERE k=? LIMIT 1");
            return s.as<Document>(k);
        }

        Document            document(std::initializer_list<std::string_view> keys)
        {
            for(std::string_view k : keys){
                Document    d = document(k);
                if(d)
                    return d;
            }
            return Document{};
        }
        
        Document            document(uint64_t i)
        {
            return exists_document(i) ? Document{i} : Document();
        }


        //std::vector<Document>    documents(Directory);
        
        std::vector<Document>    documents(Folder f, unsigned opts)
        {
            if(opts & BestSort){
                if(opts & Hidden){
                    static thread_local SQ    s("SELECT id FROM Documents WHERE folder=? ORDER BY sk");
                    return s.vec<Document>(f.id);
                } else {
                    static thread_local SQ    s("SELECT id FROM Documents WHERE folder=? AND hidden=0 ORDER BY sk");
                    return s.vec<Document>(f.id);
                }
            } else {
                if(opts & Hidden){
                    static thread_local SQ    s("SELECT id FROM Documents WHERE folder=?");
                    return s.vec<Document>(f.id);
                } else {
                    static thread_local SQ    s("SELECT id FROM Documents WHERE folder=? AND hidden=0");
                    return s.vec<Document>(f.id);
                }
            }
        }
        
        std::vector<Document>    documents(Folder f, Sorted sorted)
        {
            return documents(f, Hidden | (sorted ? BestSort : 0));
        }

        size_t              documents_count(Folder f, unsigned opts)
        {
            if(opts & Hidden){
                static thread_local SQ    s("SELECT COUNT(1) FROM Documents WHERE folder=?");
                return s.size(f.id);
            } else {
                static thread_local SQ    s("SELECT COUNT(1) FROM Documents WHERE folder=? AND hidden=0");
                return s.size(f.id);
            }
        }
        
        namespace {
            std::vector<Document>    documents_by_suffix_sorted(Folder f, std::string_view sfx)
            {
                static thread_local SQ    s("SELECT id FROM Documents WHERE folder=? AND suffix=? ORDER BY k");
                return s.vec<Document>(f.id, sfx);
            }

            std::vector<Document>    documents_by_suffix_unsorted(Folder f, std::string_view sfx)
            {
                static thread_local SQ    s("SELECT id FROM Documents WHERE folder=? AND suffix=?");
                return s.vec<Document>(f.id, sfx);
            }
        }
        
        std::vector<Document>    documents_by_suffix(Folder f, std::string_view sfx, Sorted sorted)
        {
            return sorted ? documents_by_suffix_sorted(f, sfx) : documents_by_suffix_unsorted(f,sfx);
        }
        
        
        namespace {
            std::vector<Document>    documents_by_suffix_excluding_sorted(Folder f, std::string_view sfx)
            {
                static thread_local SQ    s("SELECT id FROM Documents WHERE folder!=? AND suffix=? ORDER BY k");
                return s.vec<Document>(f.id, sfx);
            }
            
            std::vector<Document>    documents_by_suffix_excluding_unsorted(Folder f, std::string_view sfx)
            {
                static thread_local SQ    s("SELECT id FROM Documents WHERE folder!=? AND suffix=?");
                return s.vec<Document>(f.id, sfx);
            }
        }
        
        std::vector<Document>    documents_by_suffix_excluding(Folder f, std::string_view sfx, Sorted sorted)
        {
            return sorted ? documents_by_suffix_excluding_sorted(f,sfx) : documents_by_suffix_excluding_unsorted(f,sfx);
        }

        void    erase(Directory d)
        {
            if(!d)
                return ;
            
            Folder          f   = folder(d);
            if(f.id && f.id <= kMaxFixedFolder)
                return ;
            
            {
                static thread_local SQ dq("DELETE FROM Directories WHERE id=?");
                dq.bind(1, d.id);
                dq.step();
                dq.reset();
            }
            
            if(!f)
                return ;
            
            size_t dc = directories_count(f);
            if(!dc){
                static thread_local SQ dq("DELETE FROM Folders WHERE id=?");
                dq.bind(1, f.id);
                dq.step();
                dq.reset();
            }
        }

        void    erase(Fragment f)
        {
            if(!f)
                return ;
                
            Document    doc = document(f);
            {
                static thread_local SQ d("DELETE FROM Fragments WHERE id=?");
                d.bind(1, f.id);
                d.step();
                d.reset();
            }
            
            if(!doc)
                return ;
            
            size_t  fc  = fragments_count(doc);
            if(!fc){
                static thread_local SQ d("DELETE FROM Documents WHERE id=?");
                d.bind(1, doc.id);
                d.step();
                d.reset();
            }
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
                
            static thread_local SQ s("SELECT 1 FROM Directories WHERE folder=? AND root=? LIMIT 1");
            return s.present(fo.id, rt->id);
        }
        
        bool                exists(Fragment f)
        {
            return exists_fragment(f.id);
        }
        
        bool                exists(const Root*rt, std::string_view z)
        {
            if(!rt)
                return false;
            return std::filesystem::exists(rt -> resolve(z));
        }
        
        bool                exists_directory(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Directories WHERE id=? LIMIT 1");
            return s.present(i);
        }

        bool                exists_document(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Documents WHERE id=? LIMIT 1");
            return s.present(i);
        }

        bool                exists_folder(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Folders WHERE id=? LIMIT 1");
            return s.present(i);
        }

        bool                exists_fragment(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Fragments WHERE id=? LIMIT 1");
            return s.present(i);
        }

        Fragment            first(Document d)
        {
            static thread_local SQ s("SELECT id FROM Fragments WHERE document=? ORDER BY root LIMIT 1");
            return s.as<Fragment>(d.id);
        }
        
        Fragment            first(Document d, const Root*rt)
        {
            if(!rt)
                return Fragment{};
            static thread_local SQ s("SELECT id FROM Fragments WHERE document=? AND root=? LIMIT 1");
            return s.as<Fragment>(d.id, rt->id);
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
            static thread_local SQ    s("SELECT folder FROM Directories WHERE id=?");
            return s.as<Folder>(d.id);
        }
        
        Folder              folder(Document d)
        {
            static thread_local SQ    s("SELECT folder FROM Documents WHERE id=? LIMIT 1");
            return s.as<Folder>(d.id);
        }
        
        Folder              folder(Fragment f)
        {
            static thread_local SQ    s("SELECT folder FROM Fragments WHERE id=?");
            return s.as<Folder>(f.id);
        }
        
        Folder              folder(Folder f, std::string_view ck)
        {
            static thread_local SQ    s("SELECT id FROM Folders WHERE parent=? AND ck=? LIMIT 1");
            return s.as<Folder>(f.id, ck);
        }
        

        Folder              folder(uint64_t i)
        {
            return exists_folder(i) ? Folder{i} : Folder{};
        }
        
        Folder              folder(std::string_view k)
        {
            static thread_local SQ    s("SELECT id FROM Folders WHERE k=? LIMIT 1");
            return s.as<Folder>(k);
        }

        std::vector<Folder>      folder_path(Directory d)
        {
            return folder_path(folder(d));
        }

        std::vector<Folder>      folder_path(Document doc)
        {
            std::vector<Folder>  ret;
            for(Folder f = parent(doc); f; f = parent(f))
                ret.push_back(f);
            return std::vector<Folder>(ret.rbegin(), ret.rend());
        }
        
        std::vector<Folder>      folder_path(Folder folder)
        {
            std::vector<Folder>  ret;
            for(Folder f = parent(folder); f; f = parent(f))
                ret.push_back(f);
            return std::vector<Folder>(ret.rbegin(), ret.rend());
        }

        std::vector<Folder>      folder_path(Fragment f)
        {
            return folder_path(document(f));
        }
        
        std::vector<Folder>      folders(Folder f, unsigned opts)
        {
            if(opts & BestSort){
                if(opts & Hidden){
                    static thread_local SQ    s("SELECT id FROM Folders WHERE parent=? ORDER BY sk");
                    return s.vec<Folder>(f.id);
                } else {
                    static thread_local SQ    s("SELECT id FROM Folders WHERE parent=? AND hidden=0 ORDER BY sk");
                    return s.vec<Folder>(f.id);
                }
            } else {
                if(opts & Hidden){
                    static thread_local SQ    s("SELECT id FROM Folders WHERE parent=?");
                    return s.vec<Folder>(f.id);
                } else {
                    static thread_local SQ    s("SELECT id FROM Folders WHERE parent=? AND hidden=0");
                    return s.vec<Folder>(f.id);
                }
            }
        }

        std::vector<Folder>      folders(Folder f, Sorted sorted)
        {
            return folders(f, Hidden | (sorted ? BestSort : 0));
        }
        
        
        size_t              folders_count(Folder f, unsigned opts)
        {
            if(opts & Hidden){
                static thread_local SQ    s("SELECT COUNT(1) FROM Folders WHERE parent=?");
                return s.size(f.id);
            } else {
                static thread_local SQ    s("SELECT COUNT(1) FROM Folders WHERE parent=? AND hidden=0");
                return s.size(f.id);
            }
        }

        ByteArray           frag_bytes(Fragment f)
        {
            return file_bytes(path(f));
        }
        
        ByteArray           frag_bytes(std::string_view k)
        {
            return frag_bytes(fragment(document(k)));
        }
        
        ByteArray           frag_bytes(std::string_view k, DataRole dr)
        {
            return frag_bytes(fragment(document(k),dr));
        }
        
        
        std::string              frag_string(Fragment f)
        {
            return file_string(path(f));
        }
        
        std::string              frag_string(std::string_view k)
        {
            return frag_string(fragment(document(k)));
        }
        
        std::string              frag_string(std::string_view k, DataRole dr)
        {
            return frag_string(fragment(document(k),dr));
        }

        Fragment            fragment(const std::filesystem::path&k)
        {
            static thread_local SQ   s("SELECT id FROM Fragments WHERE path=? LIMIT 1");
            return s.as<Fragment>(k);
        }

        Fragment            fragment(std::string_view k)
        {
            static thread_local SQ    s("SELECT id FROM Fragments WHERE path=? LIMIT 1");
            return s.as<Fragment>(k);
        }
        
        Fragment            fragment(uint64_t i)
        {
            return exists_fragment(i) ? Fragment{i} : Fragment{};
        }
        
        Fragment            fragment(const Root*rt, std::string_view z)
        {
            if(!rt)
                return Fragment{};
            return fragment(rt->resolve(z));
        }
        
        
        Fragment            fragment(Directory d, std::string_view k)
        {
            static thread_local SQ s("SELECT id FROM Fragments WHERE dir=? AND name=? LIMIT 1");
            return s.as<Fragment>(d.id, k);
        }
        
        Fragment            fragment(Document d)
        {
            static thread_local SQ    s("SELECT id FROM Fragments WHERE document=? LIMIT 1");
            return s.as<Fragment>(d.id);
        }
        
        Fragment            fragment(Document d, const Root* rt)
        {
            if(!rt)
                return Fragment{};
            static thread_local SQ    s("SELECT id FROM Fragments WHERE document=? AND root=? LIMIT 1");
            return s.as<Fragment>(d.id, rt->id);
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
            std::vector<Fragment>    fragments_sorted(Document d)
            {
                static thread_local SQ    s("SELECT id FROM Fragments WHERE document=? ORDER BY path");
                return s.vec<Fragment>(d.id);
            }

            std::vector<Fragment>    fragments_unsorted(Document d)
            {
                static thread_local SQ    s("SELECT id FROM Fragments WHERE document=?");
                return s.vec<Fragment>(d.id);
            }
        }

        std::vector<Fragment>    fragments(Document d, Sorted sorted)
        {
            return sorted ? fragments_sorted(d) : fragments_unsorted(d);
        }
        
        std::vector<Fragment>        fragments(Document d, Sorted::Value sorted)
        {
            return sorted ? fragments_sorted(d) : fragments_unsorted(d);
        }
        
        namespace {
            std::vector<Fragment>    fragments_sorted(Document d, const Root* rt)
            {
                static thread_local SQ    s("SELECT id FROM Fragments WHERE document=? AND root=? ORDER BY path");
                return s.vec<Fragment>(d.id, rt->id);
            }

            std::vector<Fragment>    fragments_unsorted(Document d, const Root* rt)
            {
                static thread_local SQ    s("SELECT id FROM Fragments WHERE document=? AND root=?");
                return s.vec<Fragment>(d.id, rt->id);
            }
        }
        
        std::vector<Fragment>    fragments(Document d, const Root* rt, Sorted sorted)
        {
            if(!rt)
                return std::vector<Fragment>();
            return sorted ? fragments_sorted(d,rt) : fragments_unsorted(d,rt);
        }
        
        std::vector<Fragment>    fragments(Document d, const Root* rt, Sorted::Value sorted)
        {
            return fragments(d, rt, Sorted{sorted});
        }

        std::vector<Fragment>    fragments(Document d, DataRole dr, Sorted sorted)
        {
            std::vector<Fragment>    ret;
            for(const Root* rt : wksp::root_reads()[dr])
                ret += fragments(d,rt,sorted);
            return ret;
        }

        namespace {
            std::vector<Fragment>    fragments_sorted(Directory d)
            {
                static thread_local SQ    s("SELECT id FROM Fragments WHERE dir=? ORDER BY path");
                return s.vec<Fragment>(d.id);
            }

            std::vector<Fragment>    fragments_unsorted(Directory d)
            {
                static thread_local SQ    s("SELECT id FROM Fragments WHERE dir=?");
                return s.vec<Fragment>(d.id);
            }
        }
        
        std::vector<Fragment>    fragments(Directory d, Sorted sorted)
        {
            return sorted ? fragments_sorted(d) : fragments_unsorted(d);
        }
        
        namespace {
            std::vector<Fragment>    fragments_sorted(Folder f)
            {
                static thread_local SQ    s("SELECT id FROM Fragments WHERE folder=? ORDER BY path");
                return s.vec<Fragment>(f.id);
            }

            std::vector<Fragment>    fragments_unsorted(Folder f)
            {
                static thread_local SQ    s("SELECT id FROM Fragments WHERE folder=?");
                return s.vec<Fragment>(f.id);
            }
        }
        
        std::vector<Fragment>    fragments(Folder f, Sorted sorted)
        {
            return sorted ? fragments_sorted(f) : fragments_unsorted(f);
        }
        
        std::vector<Fragment>    fragments(const Root*rt, Sorted sorted)
        {
            return fragments(directory(rt), sorted);
        }
        
        std::vector<Fragment>    fragments(std::string_view k, Sorted sorted)
        {
            return fragments(document(k),sorted);
        }
        
        std::vector<Fragment>    fragments(std::string_view k, DataRole dr, Sorted sorted)
        {
            return fragments(document(k), dr, sorted);
        }

        size_t              fragments_count(Document d)
        {
            static thread_local SQ    s("SELECT COUNT(1) FROM Fragments WHERE document=?");
            return s.size(d.id);
        }
        
        size_t              fragments_count(Document d, const Root*rt)
        {
            if(!rt)
                return 0;
                
            static thread_local SQ    s("SELECT COUNT(1) FROM Fragments WHERE document=? AND root=?");
            return s.size(d.id, rt->id);
        }
        
        size_t              fragments_count(Directory d)
        {
            static thread_local SQ        s("SELECT COUNT(1) FROM Fragments WHERE dir=?");
            return s.size(d.id);
        }
        
        size_t              fragments_count(Folder f)
        {
            static thread_local SQ        s("SELECT COUNT(1) FROM Fragments WHERE folder=?");
            return s.size(f.id);
        }

        std::set<Fragment>    fragments_set(Directory d)
        {
            static thread_local SQ    s("SELECT id FROM Fragments WHERE dir=?");
            return s.set<Fragment>(d.id);
        }

        bool                hidden(Directory d)
        {
            static thread_local SQ    s("SELECT hidden FROM Directories WHERE id=?");
            return s.boolean(d.id);
        }
        
        bool                hidden(Document d)
        {
            static thread_local SQ    s("SELECT hidden FROM Documents WHERE id=? LIMIT 1");
            return s.boolean(d.id);
        }
        
        bool                hidden(Folder f)
        {
            static thread_local SQ    s("SELECT hidden FROM Folders WHERE id=?");
            return s.boolean(f.id);
        }
        
        bool                hidden(Fragment f)
        {
            static thread_local SQ    s("SELECT hidden FROM Fragments WHERE id=?");
            return s.boolean(f.id);
        }
        
        void                hide(Document d)
        {
            static thread_local SQ u("UPDATE Documents SET hidden=1 WHERE id=?");
            u.bind(1, d.id);
            u.step();
            u.reset();
        }
        
        Image               icon(Document d) 
        {
            static thread_local SQ    s("SELECT icon FROM Documents WHERE id=? LIMIT 1");
            return s.as<Image>(d.id);
        }

        Image               icon(Folder f)
        {
            static thread_local SQ    s("SELECT icon FROM Folders WHERE id=? LIMIT 1");
            return s.as<Image>(f.id);
        }

        Directory::Info     info(Directory d)
        {
            Directory::Info        ret;
            static thread_local SQ    s("SELECT folder, name, parent, path, removed, root, hidden FROM Directories WHERE id=?");
            s.bind(1, d.id);
            if(s.step() == SqlQuery::Row){
                ret.folder  = Folder(s.v_uint64(1));
                ret.name    = s.v_text(2);
                ret.parent  = Directory(s.v_uint64(3));
                ret.path    = s.v_text(4);
                ret.removed = s.v_bool(5);
                ret.root    = wksp::root(s.v_uint64(6));
                ret.hidden  = s.v_bool(7);
            }
            s.reset();
            return ret;
        }
        
        Document::Info      info(Document d)
        {
            Document::Info        ret;
            static thread_local SQ    s("SELECT k, sk, name, base, folder, suffix, removed, hidden, icon FROM Documents WHERE id=?");
            s.bind(1, d.id);
            if(s.step() == SqlQuery::Row){
                ret.key     = s.v_text(1);
                ret.skey    = s.v_text(2);
                ret.name    = s.v_text(3);
                ret.base    = s.v_text(4);
                ret.folder  = Folder(s.v_uint64(5));
                ret.suffix  = s.v_text(6);
                ret.removed = s.v_bool(7);
                ret.hidden  = s.v_bool(8);
                ret.icon    = Image(s.v_uint64(9));
            }
            s.reset();
            return ret;
        }
        
        Folder::Info        info(Folder f)
        {
            Folder::Info        ret;
            static thread_local SQ    s("SELECT k, sk, parent, name, brief, hidden, removed, icon FROM Folders WHERE id=?");
            s.bind(1,f.id);
            if(s.step() == SqlQuery::Row){
                ret.key         = s.v_text(1);
                ret.skey        = s.v_text(2);
                ret.parent      = Folder(s.v_uint64(3));
                ret.name        = s.v_text(4);
                ret.brief       = s.v_text(5);
                ret.hidden      = s.v_bool(6);
                ret.removed     = s.v_bool(7);
                ret.icon        = Image(s.v_uint64(8));
            }
            s.reset();
            return ret;
        }
        
        Fragment::Info      info(Fragment f)
        {
            Fragment::Info        ret;

            static thread_local SQ    s("SELECT document, dir, folder, modified, name, path, removed, rescan, bytes, hidden, root FROM Fragments WHERE id=?");
            s.bind(1, f.id);
            if(s.step() == SqlQuery::Row){
                ret.document    = Document(s.v_uint64(1));
                ret.directory   = Directory(s.v_uint64(2));
                ret.folder      = Folder(s.v_uint64(3));
                ret.modified    = s.v_uint64(4);
                ret.name        = s.v_text(5);
                ret.path        = s.v_text(6);
                ret.removed     = s.v_bool(7);
                ret.rescan      = s.v_bool(8);
                ret.size        = s.v_uint64(9);
                ret.hidden      = s.v_bool(10);
                ret.root        = wksp::root(s.v_uint64(11));
            }
            s.reset();
            return ret;
        }

        std::string             key(Directory d)
        {
            return key(folder(d));
        }
        
        std::string             key(Document d)
        {
            static thread_local SQ    s("SELECT k FROM Documents WHERE id=? LIMIT 1");
            return s.str(d.id);
        }

        std::string             key(Folder f) 
        {
            static thread_local SQ    s("SELECT k FROM Folders WHERE id=?");
            return s.str(f.id);
        }
        
        std::string             key(Fragment f)
        {
            return key(document(f));
        }

        std::string             key(const Root*rt)
        {
            if(!rt)
                return std::string();
            return rt->key;
        }

        //std::string             label(Directory);
        std::string             label(Document d)
        {
            return key(d);
        }

        std::string             label(Folder f)
        {
            return key(f);
        }
        
        std::string             label(Fragment f)
        {
            return path(f).string();
        }
        
        void    make_directory(Folder fo, const Root* rt)
        {
            if(!rt)
                return ;
            if(fo == top_folder())
                return ;
                
            make_path(rt->resolve(key(fo)));
        }

        uint64_t                modified(Fragment f)
        {
            static thread_local SQ    s("SELECT modified FROM Fragments WHERE id=?");
            return s.u64(f.id);
        }

        std::string             name(Directory d)
        {
            static thread_local SQ    s("SELECT name FROM Directories WHERE id=?");
            return s.str(d.id);
        }
        
        std::string             name(Document d)
        {
            static thread_local SQ    s("SELECT name FROM Documents WHERE id=? LIMIT 1");
            return s.str(d.id);
        }

        std::string             name(Folder f)
        {
            static thread_local SQ    s("SELECT name FROM Folders WHERE id=?");
            return s.str(f.id);
        }
        
        std::string             name(Fragment f)
        {
            static thread_local SQ    s("SELECT name FROM Fragments WHERE id=?");
            return s.str(f.id);
        }

        NKI                 nki(Document d, bool autoKey)
        {
            static thread_local SQ    s("SELECT name,icon,k FROM Documents WHERE id=?");
            auto s_af = s.af();
            s.bind(1, d.id);
            if(s.step() == SqlQuery::Row){
                NKI  ret;
                ret.name    = s.v_text(1);
                ret.icon    = Image(s.v_uint64(2)) ;
                ret.key     = s.v_text(3);
                if(autoKey && ret.name.empty())
                    ret.name    = ret.key;
                return ret;
            }
            return NKI{};
        }

        NKI                 nki(Folder f, bool autoKey)
        {
            static thread_local SQ    s("SELECT name,icon,k FROM Folders WHERE id=?");
            auto s_af = s.af();
            s.bind(1, f.id);
            if(s.step() == SqlQuery::Row){
                NKI  ret;
                ret.name    = s.v_text(1);
                ret.icon    = Image(s.v_uint64(2)) ;
                ret.key     = s.v_text(3);
                if(autoKey && ret.name.empty())
                    ret.name    = ret.key;
                return ret;
            }
            return NKI{};
        }

        Directory           parent(Directory d)
        {
            static thread_local SQ    s("SELECT parent FROM Directories WHERE id=?");
            return s.as<Directory>(d.id);
        }
        
        Folder              parent(Document d)
        {
            return folder(d);
        }
        
        Folder              parent(Folder f)
        {
            static thread_local SQ    s("SELECT parent FROM Folders WHERE id=?");
            return s.as<Folder>(f.id);
        }
        
        Directory           parent(Fragment f)
        {
            return directory(f);
        }

        std::filesystem::path   path(Directory d)
        {
            static thread_local SQ    s("SELECT path FROM Directories WHERE id=?");
            return s.path(d.id);
        }
        
        std::filesystem::path   path(Fragment f)
        {
            static thread_local SQ    s("SELECT path FROM Fragments WHERE id=?");
            return s.path(f.id);
        }
        
        std::filesystem::path   path(const Root*rt, std::string_view z, bool fMakePath)
        {
            std::filesystem::path   p =  rt -> resolve(z);
            if(fMakePath && !p.empty())
                make_parent_path(p);
            return p;
        }
        
        std::filesystem::path   path(const Root*rt, Document doc, bool fMakePath)
        {
            return path(rt, key(doc), fMakePath);
        }
        
        bool                removed(Directory d)
        {
            static thread_local SQ    s("SELECT removed FROM Directories WHERE id=?");
            return s.boolean(d.id);
        }
        
        bool                removed(Document d)
        {
            static thread_local SQ    s("SELECT removed FROM Documents WHERE id=?");
            return s.boolean(d.id);
        }
        
        bool                removed(Folder f)
        {
            static thread_local SQ    s("SELECT removed FROM Folders WHERE id=?");
            return s.boolean(f.id);
        }
        
        bool                removed(Fragment f)
        {
            static thread_local SQ    s("SELECT removed FROM Fragments WHERE id=?");
            return s.boolean(f.id);
        }
        
        void                rescan(Fragment f)
        {
            static thread_local SQ    u("UPDATE Fragments SET rescan=1 WHERE id=?");
            u.bind(1, f.id);
            u.step();
            u.reset();
        }
        
        bool                rescanning(Fragment f)
        {
            static thread_local SQ    s("SELECT rescan FROM Fragments WHERE id=?");
            return s.boolean(f.id);
        }

        const Root*         root(Directory d)
        {
            static thread_local SQ    s("SELECT root FROM Directories WHERE id=?");
            auto s_lk   = s.af();
            s.bind(1, d.id);
            if(s.step() == SqlQuery::Row)
                return wksp::root(s.v_uint64(0));
            return nullptr;
        }
        
        const Root*         root(Fragment f)
        {
            static thread_local SQ    s("SELECT root FROM Fragments WHERE id=?");
            auto s_af   = s.af();
            s.bind(1, f.id);
            if(s.step() == SqlQuery::Row)
                return wksp::root(s.v_uint64(0));
            return nullptr;
        }

        std::vector<const Root*> roots(Document d)
        {
            std::vector<const Root*> ret;
            static thread_local SQ    s("SELECT DISTINCT root FROM Fragments WHERE document=?");
            auto s_af       = s.af();
            s.bind(1, d.id);
            while(s.step() == SqlQuery::Row){
                const Root*r    = wksp::root(s.v_uint64(1));
                if(r)
                    ret.push_back(r);
            }
            return ret;
        }
        
        std::vector<const Root*> roots(Folder f)
        {
            std::vector<const Root*> ret;
            static thread_local SQ    s("SELECT DISTINCT root FROM Directories WHERE folder=?");
            auto s_af       = s.af();
            s.bind(1, f.id);
            while(s.step() == SqlQuery::Row){
                const Root*r    = wksp::root(s.v_uint64(1));
                if(r)
                    ret.push_back(r);
            }
            return ret;
        }
        

        size_t              roots_count(Document d)
        {
            static thread_local SQ    s("SELECT COUNT(DISTINCT root) FROM Fragments WHERE document=?");
            return s.size(d.id);
        }
        
        size_t              roots_count(Folder f)
        {
            static thread_local SQ    s("SELECT COUNT(DISTINCT root) FROM Directories WHERE folder=?");
            return s.size(f.id);
        }
        
        void                show(Document d)
        {
            static thread_local SQ u("UPDATE Documents SET hidden=0 WHERE id=?");
            auto u_af   = u.af();
            u.bind(1, d.id);
            u.step();
        }

        size_t              size(Fragment f)
        {
            static thread_local SQ    s("SELECT bytes FROM Fragments WHERE id=?");
            return s.size(f.id);
        }

        std::string             skey(Directory d)
        {
            static thread_local SQ    s("SELECT name FROM Documents WHERE id=?");
            return s.str(d.id);
        }
        
        std::string             skey(Document d)
        {
            static thread_local SQ    s("SELECT sk FROM Documents WHERE id=?");
            return s.str(d.id);
        }
        
        std::string             skey(Folder f)
        {
            static thread_local SQ    s("SELECT sk FROM Folders WHERE id=?");
            return s.str(f.id);
        }
        
        std::string             skey(Fragment f)
        {
            static thread_local SQ    s("SELECT name FROM Fragments WHERE id=?");
            return s.str(f.id);
        }

        std::string             skeyb(Directory d)
        {
            std::string     bk  = skey(d);
            return copy(base_key(bk));
        }
        
        std::string             skeyb(Document d)
        {
            std::string     bk  = skey(d);
            return copy(base_key(bk));
        }
        
        std::string             skeyb(Folder f)
        {
            std::string     bk  = skey(f);
            return copy(base_key(bk));
        }
        
        std::string             skeyb(Fragment f)
        {
            std::string     bk  = skey(f);
            return copy(base_key(bk));
        }
        

        std::string             suffix(Document d)
        {
            static thread_local SQ    s("SELECT suffix FROM Documents WHERE id=?");
            return s.str(d.id);
        }

        std::string             suffix(Fragment f)
        {
            static thread_local SQ    s("SELECT suffix FROM Fragments WHERE id=?");
            return s.str(f.id);
        }

        void                update(Fragment f)
        {
            std::filesystem::path   p           = path(f);
            SizeTimestamp   sz  = file_size_and_timestamp(p.c_str());
        
            static thread_local SQ    u("UPDATE Fragments SET bytes=?,modified=?,removed=?,rescan=0 WHERE id=?");
            u.bind(1, sz.size);
            u.bind(2, sz.nanoseconds());
            u.bind(3, !sz.exists);
            u.bind(4, f.id);
            u.step(true);
            u.reset();
        }

        bool                within(Folder p, Directory d, bool recursive)
        {
            return within(p, folder(d), recursive);
        }
        
        bool                within(Folder p, Document d, bool recursive)
        {
            if(!p || !d)    
                return false;
            Folder  c   = parent(d);
            if(p == c)
                return true;
            if(recursive){
                while((c = parent(c)))
                    if(p == c)
                        return true;
            }
            return false;
        }
        
        bool                within(Folder p, Folder f, bool recursive)
        {
            if(!p || !f)    
                return false;
            Folder  c   = parent(f);
            if(p == c)
                return true;
            if(recursive){
                while((c = parent(c)))
                    if(p == c)
                        return true;
            }
            return false;
        }
        
        bool                within(Folder p, Fragment f, bool recursive)
        {
            return within(p, document(f), true);
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
}
