////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Directory.hpp"
#include "DirectoryArg.hpp"
#include "DirectoryCDB.hpp"
#include "DirectoryHtml.hpp"

#include "Document.hpp"
#include "DocumentArg.hpp"
#include "DocumentCDB.hpp"
#include "DocumentHtml.hpp"

#include "Folder.hpp"
#include "FolderArg.hpp"
#include "FolderCDB.hpp"
#include "FolderHtml.hpp"

#include "Fragment.hpp"
#include "FragmentArg.hpp"
#include "FragmentCDB.hpp"
#include "FragmentHtml.hpp"

#include "Root.hpp"
#include "RootArg.hpp"
#include "RootCDB.hpp"
#include "RootHtml.hpp"

#include <yq/collection/c_utils.hpp>
#include <yq/db/IDLock.hpp>
#include <yq/db/NKI.hpp>
#include <yq/db/SQ.hpp>
#include <yq/image/Image.hpp>
#include <yq/image/ImageCDB.hpp>
#include <yq/io/dir_utils.hpp>
#include <yq/io/file_utils.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/text/text_utils.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>
#include <yq/wksp/Workspace.hpp>


#include <yq/db/IDLock.ipp>

namespace yq {
    template class IDLock<Fragment>;

////////////////////////////////////////////////////////////////////////////////
//  DIRECTORY
////////////////////////////////////////////////////////////////////////////////

    namespace arg {

        Directory directory(std::string_view  arg_string, const Root* rt)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Directory{};
                
            Folder      folder = folder_key(arg_string);
            if(folder)
                return cdb::first_directory(folder, rt);

            return directory_id(arg_string);
        }
        
        Directory directory(const WebContext&ctx, const Root* rt, bool *detected)
        {
            std::string    k;
            
            if(detected)
                *detected   = false;
            
            k    = ctx.find_query("id");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return directory_id(k);
            }
            
            k   = ctx.find_query("root");
            if(!k.empty())
                rt  = root(k);

            k       = ctx.find_query("key");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return directory_key(k, rt);
            }
            
            k       = ctx.find_query("directory");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return directory(k, rt);
            }
            return Directory{};
        }
        
        Directory directory(const WebContext&ctx, std::string_view arg_name, const Root* rt, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected = !arg_string.empty();
            return directory(arg_string, rt);
        }
        
        Directory directory(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, const Root* rt, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected = !arg_string.empty();
            return directory(arg_string, rt);
        }
        
        Directory directory_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_directory(i))
                return Directory{i};
            return Directory{};
        }

        Directory directory_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected = !arg_string.empty();
            return directory_id(arg_string);
        }
        
        Directory directory_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected = !arg_string.empty();
            return directory_id(arg_string);
        }
        
        
        Directory directory_key(std::string_view arg_string, const Root* rt)
        {
            Folder  folder  = folder_key(arg_string);
            if(!folder)
                return Directory();
            return cdb::first_directory(folder, rt);
        }
        
        Directory directory_key(const WebContext&ctx, std::string_view arg_name, const Root* rt, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected = !arg_string.empty();
            return directory_key(arg_string, rt);
        }
        
        Directory directory_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, const Root* rt, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected = !arg_string.empty();
            return directory_key(arg_string, rt);
        }
    }

////////////////////////////////////////////////////////////////////////////////

    namespace cdb {
        std::vector<Directory>   all_directories(unsigned opts)
        {
            if(opts & BEST_SORT){
                if(opts & HIDDEN){
                    static thread_local SQ    s("SELECT id FROM Directories ORDER BY path");
                    return s.vec<Directory>();
                } else {
                    static thread_local SQ    s("SELECT id FROM Directories WHERE hidden=0 ORDER BY path");
                    return s.vec<Directory>();
                }
            } else {
                if(opts & HIDDEN){
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
            return all_directories(HIDDEN | (sorted ? BEST_SORT : 0));
        }

        size_t              all_directories_count()
        {
            static thread_local SQ    s("SELECT COUNT(1) FROM Directories");
            return s.size();
        }
        
        DirOrFrag           child(Directory d, std::string_view ck)
        {
            Fragment    f   = child_fragment(d, ck);
            if(f)
                return f;
            Directory   dir = child_directory(d, ck);
            if(dir)
                return dir;
            return false;
        }

        size_t              child_count(Directory d)
        {
            return child_directories_count(d) + child_fragments_count(d);
        }

        Directory           child_directory(Directory d, std::string_view k)
        {
            static thread_local SQ s("SELECT id FROM Directories WHERE parent=? AND name=?");
            return s.as<Directory>(d.id, k);
        }

        Fragment            child_fragment(Directory d, std::string_view k)
        {
            static thread_local SQ s("SELECT id FROM Fragments WHERE dir=? AND name=? LIMIT 1");
            return s.as<Fragment>(d.id, k);
        }

    #if 0
        std::string                 child_key(Directory);       //<! Key inside the directory (with extensions)
    #endif

        namespace {
            std::vector<Directory>   child_directories_sorted(Directory d)
            {
                static thread_local SQ    s("SELECT id FROM Directories WHERE parent=? ORDER BY name");
                return s.vec<Directory>(d.id);
            }

            std::vector<Directory>   child_directories_unsorted(Directory d)
            {
                static thread_local SQ    s("SELECT id FROM Directories WHERE parent=?");
                return s.vec<Directory>(d.id);
            }
        }
        
        std::vector<Directory>   child_directories(Directory d, Sorted sorted)
        {
            return sorted ? child_directories_sorted(d) : child_directories_unsorted(d);
        }
        
        size_t              child_directories_count(Directory d)
        {
            static thread_local SQ    s("SELECT COUNT(1) FROM Directories WHERE parent=?");
            return s.size(d.id);
        }
        
        Set<Directory>             child_directories_set(Directory d)
        {
            static thread_local SQ    s("SELECT id FROM Directories WHERE parent=?");
            return s.set<Directory>(d.id);
        }

        namespace {
            std::vector<DirString>   child_directories_with_names_sorted(Directory dir)
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
            
            std::vector<DirString>   child_directories_with_names_unsorted(Directory dir)
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

        std::vector<DirString>   child_directories_with_names(Directory dir, Sorted sorted)
        {
            return sorted ? child_directories_with_names_sorted(dir) : child_directories_with_names_unsorted(dir);
        }

        namespace {
            std::vector<Fragment>    child_fragments_sorted(Directory d)
            {
                static thread_local SQ    s("SELECT id FROM Fragments WHERE dir=? ORDER BY path");
                return s.vec<Fragment>(d.id);
            }

            std::vector<Fragment>    child_fragments_unsorted(Directory d)
            {
                static thread_local SQ    s("SELECT id FROM Fragments WHERE dir=?");
                return s.vec<Fragment>(d.id);
            }
        }
        
        std::vector<Fragment>    child_fragments(Directory d, Sorted sorted)
        {
            return sorted ? child_fragments_sorted(d) : child_fragments_unsorted(d);
        }

        size_t              child_fragments_count(Directory d)
        {
            static thread_local SQ        s("SELECT COUNT(1) FROM Fragments WHERE dir=?");
            return s.size(d.id);
        }

        Set<Fragment>    child_fragments_set(Directory d)
        {
            static thread_local SQ    s("SELECT id FROM Fragments WHERE dir=?");
            return s.set<Fragment>(d.id);
        }

        std::vector<DirOrFrag>   children(Directory d, Sorted sorted)
        {
            std::vector<DirOrFrag>   ret;
            for(Directory d2 : child_directories(d, sorted))
                ret.push_back(d2);
            for(Fragment f : child_fragments(d, sorted))
                ret.push_back(f);
            return ret;
        }
        
        std::vector<DirOrFragStr>    children_and_names(Directory);  // TODO

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
        
        //Fragment                db_fragment(Document, const Root*); // TODO

        Directory           directory(uint64_t i)
        {
            return exists_directory(i) ? Directory{i} : Directory();
        }

        Directory           directory(const std::filesystem::path&p)
        {
            if(p.empty())
                return Directory();
            static thread_local SQ    s("SELECT id FROM Directories WHERE path=? LIMIT 1");
            return s.as<Directory>(p);
        }
        
        //std::vector<Document>    documents(Directory);

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

        bool                exists(Directory d)
        {
            return exists_directory(d.id);
        }
        
        bool                exists_directory(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Directories WHERE id=? LIMIT 1");
            return s.present(i);
        }

        Folder              folder(Directory d)
        {
            static thread_local SQ    s("SELECT folder FROM Directories WHERE id=?");
            return s.as<Folder>(d.id);
        }

        std::vector<Folder>      folder_path(Directory d)
        {
            return folder_path(folder(d));
        }


        bool                hidden(Directory d)
        {
            static thread_local SQ    s("SELECT hidden FROM Directories WHERE id=?");
            return s.boolean(d.id);
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

        std::string             key(Directory d)
        {
            return key(folder(d));
        }

        //std::string             label(Directory);

        std::string             name(Directory d)
        {
            static thread_local SQ    s("SELECT name FROM Directories WHERE id=?");
            return s.str(d.id);
        }
        
        Directory           parent(Directory d)
        {
            static thread_local SQ    s("SELECT parent FROM Directories WHERE id=?");
            return s.as<Directory>(d.id);
        }

        std::filesystem::path   path(Directory d)
        {
            static thread_local SQ    s("SELECT path FROM Directories WHERE id=?");
            return s.path(d.id);
        }
       
        bool                removed(Directory d)
        {
            static thread_local SQ    s("SELECT removed FROM Directories WHERE id=?");
            return s.boolean(d.id);
        }
        
        const Root*         root(Directory d)
        {
            static thread_local SQ    s("SELECT root FROM Directories WHERE id=?");
            auto s_lk   = s.af();
            s.bind(1, d.id);
            if(s.step() == SqlQuery::Row)
                return wksp::root(s.v_uint64(1));
            return nullptr;
        }
        
        std::string             skey(Directory d)
        {
            static thread_local SQ    s("SELECT name FROM Documents WHERE id=?");
            return s.str(d.id);
        }

        std::string             skeyb(Directory d)
        {
            std::string     bk  = skey(d);
            return copy(base_key(bk));
        }
    }

////////////////////////////////////////////////////////////////////////////////

    namespace html {
        //WebHtml&    operator<<(WebHtml&, Directory);

        WebHtml&    operator<<(WebHtml&h, Dev<Directory> v)
        {
            std::string n       = cdb::name(v.data);
            if(n.empty())
                n   = "(no-name)";
            if(v.data)
                h << "<a href=\"/dev/directory?id=" << v.data.id << "\">";
            h << "[" << v.data.id << "] " << n;
            if(v.data)
                h << "</a>";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, DevID<Directory> v)
        {
            if(v.data)
                h << "<a href=\"/dev/directory?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }

        void    dev_table(WebHtml& h, const std::vector<Directory>& dirs)
        {
            auto _tab = h.table();
                h << "<tr><th>ID</th><th>Fragments</th><th>Children</th><th>Path</th></tr>\n";
            for(Directory d : dirs){
                h << "<tr><td>" << dev_id(d) << "</td><td>" 
                    << cdb::child_fragments_count(d) << "</td><td>" << cdb::child_directories_count(d) << "</td><td>" << cdb::path(d) << "</td></tr>\n";
            }
        }

    }

////////////////////////////////////////////////////////////////////////////////
//  DOCUMENT
////////////////////////////////////////////////////////////////////////////////

    namespace arg {

        Document document(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Document{};
                
            Document t   = cdb::document( arg_string);
            if(t)
                return t;
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_document(i))
                return Document{i};
            return Document{};
        }
        
        Document document(const WebContext&ctx, bool *detected)
        {
            if(detected)
                *detected   = false;
        
            std::string    k    = ctx.find_query("id");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return document_id(k);
            }
            
            k       = ctx.find_query("key");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return document_key(k);
            }
            
            k       = ctx.find_query("document");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return document(k);
            }
            return Document{};
        }
        
        Document document(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return document(arg_string);
        }
        
        Document document(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return document(arg_string);
        }
        
        Document document_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_document(i))
                return Document{i};
            return Document{};
        }

        Document document_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return document_id(arg_string);
        }
        
        Document document_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return document_id(arg_string);
        }
        
        Document document_key(std::string_view arg_string)
        {
            return cdb::document(trimmed(arg_string));
        }
        
        Document document_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return document_key(arg_string);
        }
        
        Document document_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return document_key(arg_string);
        }
    }

////////////////////////////////////////////////////////////////////////////////

    namespace cdb {
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

        std::string             base_key(Document doc)
        {
            static thread_local SQ    s("SELECT skb FROM Documents WHERE id=?");
            return s.str(doc.id);
        }

        Image   best_image(Document x)
        {
            std::string k   = skeyc(x);
            if(k.empty())
                return Image{};
                
            k += '.';
                
            //size_t  i   = k.find_last_of('.');
            //if((i != std::string::npos) && (i>0)){
                //// truncate
                //k.resize(i+1);
            //}
            
            Folder      fo  = folder(x);
            for(const char* z : Image::kSupportedExtensions){
                std::string     k2 = k + z;
                Document        dimg    = child_document(fo, k2);
                if(!dimg)
                    continue;
                Image img = image(dimg);
                if(img)
                    return img;
            }
            return Image{};
        }

    #if 0
        std::string                 child_key(Document);
    #endif

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
            std::string_view     skc     = (x != std::string_view::npos ) ? ak.substr(0,x) : ak;
                
            static thread_local SQ    i("INSERT OR FAIL INTO Documents (k,sk,name,folder,suffix,skb,hidden,mime,skc) VALUES (?,?,?,?,?,?,?,?,?)");
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
            i.bind(8,mimeTypeForExt(sfx).value());
            i.bind(9,skc);
            
            if(is_good(i.step(false))){
                if(wasCreated)
                    *wasCreated = true;
                return Document((uint64_t) i.last_id());
            } else {
                s.bind(1,k);
                if(s.step() == SqlQuery::Row)
                    return Document{s.v_uint64(1)};
                cdbError << "Unable to get document ID";
                return Document();
            }

        }

        Document            document(std::string_view k)
        {
            static thread_local SQ    s("SELECT id FROM Documents WHERE k=? LIMIT 1");
            return s.as<Document>(k);
        }
        
        Document            document(uint64_t i)
        {
            return exists_document(i) ? Document{i} : Document();
        }

        bool                exists(Document d)
        {
            return exists_document(d.id);
        }

        bool                exists(Document doc, const Root* rt)
        {
            if(!rt)
                return false;
            static thread_local SQ s("SELECT 1 FROM Fragments WHERE document=? AND root=? LIMIT 1");
            return s.present(doc.id, rt->id);
        }

        bool                exists_document(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Documents WHERE id=? LIMIT 1");
            return s.present(i);
        }

        Document            first_document(std::initializer_list<std::string_view> keys)
        {
            for(std::string_view k : keys){
                Document    d   = document(k);
                if(d)
                    return d;
            }
            return Document{};
        }

        Document            first_document(const std::vector<std::string>& keys)
        {
            for(std::string_view k : keys){
                Document    d   = document(k);
                if(d)
                    return d;
            }
            return Document{};
        }

        Fragment            first_fragment(Document d)
        {
            static thread_local SQ s("SELECT id FROM Fragments WHERE document=? ORDER BY root LIMIT 1");
            return s.as<Fragment>(d.id);
        }
        
        Fragment            first_fragment(Document d, const Root*rt)
        {
            if(!rt)
                return first_fragment(d);
                
            static thread_local SQ s("SELECT id FROM Fragments WHERE document=? AND root=? LIMIT 1");
            return s.as<Fragment>(d.id, rt->id);
        }

        Fragment            first_fragment(Document d, DataRole dr)
        {
            for(const Root* rt : wksp::root_reads()[dr]){
                Fragment f = first_fragment(d,rt);
                if(f)
                    return f;
            }
            return Fragment{};
        }
        
        Folder              folder(Document d)
        {
            static thread_local SQ    s("SELECT folder FROM Documents WHERE id=? LIMIT 1");
            return s.as<Folder>(d.id);
        }

        std::vector<Folder>      folder_path(Document doc)
        {
            std::vector<Folder>  ret;
            for(Folder f = parent(doc); f; f = parent(f))
                ret.push_back(f);
            return std::vector<Folder>(ret.rbegin(), ret.rend());
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

        bool                hidden(Document d)
        {
            static thread_local SQ    s("SELECT hidden FROM Documents WHERE id=? LIMIT 1");
            return s.boolean(d.id);
        }
        
        void                hide(Document d)
        {
            static thread_local SQ u("UPDATE Documents SET hidden=1 WHERE id=?");
            u.bind(1, d.id);
            u.exec();
            u.reset();
        }
        
        Image               icon(Document d) 
        {
            static thread_local SQ    s("SELECT icon FROM Documents WHERE id=? LIMIT 1");
            return s.as<Image>(d.id);
        }
        
        Document::Info      info(Document d)
        {
            Document::Info        ret;
            static thread_local SQ    s("SELECT k, sk, name, skb, folder, suffix, removed, hidden, icon, skc FROM Documents WHERE id=?");
            s.bind(1, d.id);
            if(s.step() == SqlQuery::Row){
                ret.key     = s.v_text(1);
                ret.skey    = s.v_text(2);
                ret.name    = s.v_text(3);
                ret.skeyb   = s.v_text(4);
                ret.folder  = Folder(s.v_uint64(5));
                ret.suffix  = s.v_text(6);
                ret.removed = s.v_bool(7);
                ret.hidden  = s.v_bool(8);
                ret.icon    = Image(s.v_uint64(9));
                ret.skeyc   = s.v_text(10);
            }
            s.reset();
            return ret;
        }
        
        std::string             key(Document d)
        {
            static thread_local SQ    s("SELECT k FROM Documents WHERE id=? LIMIT 1");
            return s.str(d.id);
        }

        std::string             label(Document d)
        {
            return key(d);
        }

        ContentType             mime_type(Document d)
        {
            static thread_local SQ  s("SELECT mime FROM Documents WHERE id=? LIMIT 1");
            auto af = s.af();
            s.bind(1, d.id);
            if(s.step() == SqlQuery::Row){
                return (ContentType::enum_t) s.v_int(1);
            } else 
                return ContentType();
        }
        
        std::string             name(Document d)
        {
            static thread_local SQ    s("SELECT name FROM Documents WHERE id=? LIMIT 1");
            return s.str(d.id);
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
        
        Folder              parent(Document d)
        {
            return folder(d);
        }
        
        bool                removed(Document d)
        {
            static thread_local SQ    s("SELECT removed FROM Documents WHERE id=?");
            return s.boolean(d.id);
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

        size_t              roots_count(Document d)
        {
            static thread_local SQ    s("SELECT COUNT(DISTINCT root) FROM Fragments WHERE document=?");
            return s.size(d.id);
        }

        void                set_mime_type(Document doc, ContentType ct)
        {
            static thread_local SQ  u("UPDATE Documents SET mime=? WHERE id=?");
            u.bind(1, ct.value());
            u.bind(2, doc.id);
            u.step(true);
            u.reset();
        }
        
        void                show(Document d)
        {
            static thread_local SQ u("UPDATE Documents SET hidden=0 WHERE id=?");
            u.bind(1, d.id);
            u.step(true);
            u.reset();
        }
        
        std::string             skey(Document d)
        {
            static thread_local SQ    s("SELECT sk FROM Documents WHERE id=?");
            return s.str(d.id);
        }
        
        std::string             skeyb(Document d)
        {
            static thread_local SQ    s("SELECT skb FROM Documents WHERE id=?");
            return s.str(d.id);
        }

        std::string             skeyc(Document d)
        {
            static thread_local SQ    s("SELECT skc FROM Documents WHERE id=?");
            return s.str(d.id);
        }

        std::string             suffix(Document d)
        {
            static thread_local SQ    s("SELECT suffix FROM Documents WHERE id=?");
            return s.str(d.id);
        }

        Fragment            writable(Document d, DataRole dr)
        {
            for(const Root* rt : wksp::root_writes()[dr]){
                Fragment f = first_fragment(d, rt);
                if(f)
                    return f;
            }
            return Fragment{};
        }
    }

////////////////////////////////////////////////////////////////////////////////

    namespace html {
        //WebHtml&    operator<<(WebHtml&, Document);

        WebHtml&    operator<<(WebHtml&h, Dev<Document> v)
        {
            std::string n       = cdb::name(v.data);
            if(n.empty())
                n   = "(no-name)";
            if(v.data)
                h << "<a href=\"/dev/document?id=" << v.data.id << "\">";
            h << "[" << v.data.id << "] " << n;
            if(v.data)
                h << "</a>";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, DevID<Document> v)
        {
            if(v.data)
                h << "<a href=\"/dev/document?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }

        void    dev_table(WebHtml& h, const std::vector<Document>& documents)
        {
            auto _tab = h.table();
            h << "<tr><th>ID</th><th>Fragments</th><th>Key</th><th>Name</th><th>Suffix</th>\n";
            for(Document a : documents){
                auto i = cdb::info(a);
                h << "<tr><td>" << dev_id(a)  << "</td><td>" << cdb::fragments_count(a) 
                  << "</td><td>" << i.key << "</td><td>" << i.name << "</td><td>" << i.suffix << "</td></tr>\n";
            }
        }
    }

////////////////////////////////////////////////////////////////////////////////
//  FOLDER
////////////////////////////////////////////////////////////////////////////////

    namespace arg {

        Folder folder(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Folder{};
                
            Folder t   = cdb::folder( arg_string);
            if(t)
                return t;
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_folder(i))
                return Folder{i};
            return Folder{};
        }
        
        Folder folder(const WebContext&ctx, bool *detected)
        {
            std::string    k    = ctx.find_query("id");
            
            if(detected)
                *detected   = false;
            
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return folder_id(k);
            }
            
            k       = ctx.find_query("key");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return folder_key(k);
            }
            
            k       = ctx.find_query("folder");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return folder(k);
            }
            return Folder{};
        }
        
        Folder folder(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected = !arg_string.empty();
            return folder(arg_string);
        }
        
        Folder folder(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected = !arg_string.empty();
            return folder(arg_string);
        }
        
        Folder folder_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_folder(i))
                return Folder{i};
            return Folder{};
        }

        Folder folder_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected = !arg_string.empty();
            return folder_id(arg_string);
        }
        
        
        Folder folder_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected = !arg_string.empty();
            return folder_id(arg_string);
        }
        
        
        Folder folder_key(std::string_view arg_string)
        {
            return cdb::folder(trimmed(arg_string));
        }

        Folder folder_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected = !arg_string.empty();
            return folder_key(arg_string);
        }
        
        Folder folder_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected = !arg_string.empty();
            return folder_key(arg_string);
        }
    }

////////////////////////////////////////////////////////////////////////////////

    namespace cdb {
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

        std::string             brief(Folder f)
        {
            static thread_local SQ    s("SELECT brief FROM Folders WHERE id=?");
            return s.str(f.id);
        }
        
        DocOrFold           child(Folder f, std::string_view ck)
        {
            Document    doc   = child_document(f, ck);
            if(doc)
                return doc;
            Folder      fold  = child_folder(f, ck);
            if(fold)
                return fold;
            return false;
        }

        size_t              child_count(Folder f)
        {
            return child_documents_count(f) + child_folders_count(f);
        }

        Document            child_document(Folder f, std::string_view k)
        {
            static thread_local SQ s("SELECT id FROM Documents WHERE folder=? AND sk=?");
            return s.as<Document>(f.id, k);
        }
        
        std::vector<Document>    child_documents(Folder f, unsigned opts)
        {
            if(opts & BEST_SORT){
                if(opts & HIDDEN){
                    static thread_local SQ    s("SELECT id FROM Documents WHERE folder=? ORDER BY sk");
                    return s.vec<Document>(f.id);
                } else {
                    static thread_local SQ    s("SELECT id FROM Documents WHERE folder=? AND hidden=0 ORDER BY sk");
                    return s.vec<Document>(f.id);
                }
            } else {
                if(opts & HIDDEN){
                    static thread_local SQ    s("SELECT id FROM Documents WHERE folder=?");
                    return s.vec<Document>(f.id);
                } else {
                    static thread_local SQ    s("SELECT id FROM Documents WHERE folder=? AND hidden=0");
                    return s.vec<Document>(f.id);
                }
            }
        }
        
        std::vector<Document>    child_documents(Folder f, Sorted sorted)
        {
            return child_documents(f, HIDDEN | (sorted ? BEST_SORT : 0));
        }

        size_t              child_documents_count(Folder f, unsigned opts)
        {
            if(opts & HIDDEN){
                static thread_local SQ    s("SELECT COUNT(1) FROM Documents WHERE folder=?");
                return s.size(f.id);
            } else {
                static thread_local SQ    s("SELECT COUNT(1) FROM Documents WHERE folder=? AND hidden=0");
                return s.size(f.id);
            }
        }
        
        namespace {
            std::vector<Document>    child_documents_by_suffix_sorted(Folder f, std::string_view sfx)
            {
                static thread_local SQ    s("SELECT id FROM Documents WHERE folder=? AND suffix=? ORDER BY k");
                return s.vec<Document>(f.id, sfx);
            }

            std::vector<Document>    child_documents_by_suffix_unsorted(Folder f, std::string_view sfx)
            {
                static thread_local SQ    s("SELECT id FROM Documents WHERE folder=? AND suffix=?");
                return s.vec<Document>(f.id, sfx);
            }
        }
        
        std::vector<Document>    child_documents_by_suffix(Folder f, std::string_view sfx, Sorted sorted)
        {
            return sorted ? child_documents_by_suffix_sorted(f, sfx) : child_documents_by_suffix_unsorted(f,sfx);
        }
        
        
        namespace {
            std::vector<Document>    child_documents_by_suffix_excluding_sorted(Folder f, std::string_view sfx)
            {
                static thread_local SQ    s("SELECT id FROM Documents WHERE folder!=? AND suffix=? ORDER BY k");
                return s.vec<Document>(f.id, sfx);
            }
            
            std::vector<Document>    child_documents_by_suffix_excluding_unsorted(Folder f, std::string_view sfx)
            {
                static thread_local SQ    s("SELECT id FROM Documents WHERE folder!=? AND suffix=?");
                return s.vec<Document>(f.id, sfx);
            }
        }
        
        std::vector<Document>    child_documents_by_suffix_excluding(Folder f, std::string_view sfx, Sorted sorted)
        {
            return sorted ? child_documents_by_suffix_excluding_sorted(f,sfx) : child_documents_by_suffix_excluding_unsorted(f,sfx);
        }

        Folder              child_folder(Folder f, std::string_view ck)
        {
            static thread_local SQ    s("SELECT id FROM Folders WHERE parent=? AND ck=? LIMIT 1");
            return s.as<Folder>(f.id, ck);
        }

        std::vector<Folder>      child_folders(Folder f, unsigned opts)
        {
            if(opts & BEST_SORT){
                if(opts & HIDDEN){
                    static thread_local SQ    s("SELECT id FROM Folders WHERE parent=? ORDER BY sk");
                    return s.vec<Folder>(f.id);
                } else {
                    static thread_local SQ    s("SELECT id FROM Folders WHERE parent=? AND hidden=0 ORDER BY sk");
                    return s.vec<Folder>(f.id);
                }
            } else {
                if(opts & HIDDEN){
                    static thread_local SQ    s("SELECT id FROM Folders WHERE parent=?");
                    return s.vec<Folder>(f.id);
                } else {
                    static thread_local SQ    s("SELECT id FROM Folders WHERE parent=? AND hidden=0");
                    return s.vec<Folder>(f.id);
                }
            }
        }

        std::vector<Folder>      child_folders(Folder f, Sorted sorted)
        {
            return child_folders(f, HIDDEN | (sorted ? BEST_SORT : 0));
        }
        
        
        size_t              child_folders_count(Folder f, unsigned opts)
        {
            if(opts & HIDDEN){
                static thread_local SQ    s("SELECT COUNT(1) FROM Folders WHERE parent=?");
                return s.size(f.id);
            } else {
                static thread_local SQ    s("SELECT COUNT(1) FROM Folders WHERE parent=? AND hidden=0");
                return s.size(f.id);
            }
        }


        namespace {
            std::vector<Fragment>    child_fragments_sorted(Folder f)
            {
                static thread_local SQ    s("SELECT id FROM Fragments WHERE folder=? ORDER BY path");
                return s.vec<Fragment>(f.id);
            }

            std::vector<Fragment>    child_fragments_unsorted(Folder f)
            {
                static thread_local SQ    s("SELECT id FROM Fragments WHERE folder=?");
                return s.vec<Fragment>(f.id);
            }
        }
        
        std::vector<Fragment>    child_fragments(Folder f, Sorted sorted)
        {
            return sorted ? child_fragments_sorted(f) : child_fragments_unsorted(f);
        }
        
        size_t              child_fragments_count(Folder f)
        {
            static thread_local SQ        s("SELECT COUNT(1) FROM Fragments WHERE folder=?");
            return s.size(f.id);
        }

    #if 0
        std::string                 child_key(Folder);
    #endif

        std::vector<DocOrFold>   children(Folder, Sorted sorted);           // TODO
        std::vector<DocOrFoldStr>    children_and_names(Folder);     // TODO
        
        
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
        
        size_t              directories_count(Folder f)
        {
            static thread_local SQ    s("SELECT COUNT(1) FROM Directories WHERE folder=?");
            return s.size(f.id);
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

        bool                exists_folder(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Folders WHERE id=? LIMIT 1");
            return s.present(i);
        }

        Directory           first_directory(Folder fo)
        {
            if(!fo)
                return Directory();
            static thread_local SQ  s("SELECT id FROM Directories WHERE folder=? LIMIT 1");
            return s.as<Directory>(fo.id);
        }
        
        Directory           first_directory(Folder fo, const Root*r)
        {
            if(!r)
                return first_directory(fo);
            if(!fo)
                return Directory();
            
            static thread_local SQ s("SELECT id FROM Directories WHERE folder=? AND root=? LIMIT 1");
            auto af = s.af();
            s.bind(1, fo.id);
            s.bind(2, r->id);
            if(s.step() == SqlQuery::Row)
                return Directory{ s.v_uint64(1)};
            return Directory{};
        }

        Document            first_document(Folder f, std::initializer_list<std::string_view> keys)
        {
            for(std::string_view k : keys){
                Document    d   = child_document(f, k);
                if(d)
                    return d;
            }
            return Document{};
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

        std::vector<Folder>      folder_path(Folder folder)
        {
            std::vector<Folder>  ret;
            for(Folder f = parent(folder); f; f = parent(f))
                ret.push_back(f);
            return std::vector<Folder>(ret.rbegin(), ret.rend());
        }
        
        bool                hidden(Folder f)
        {
            static thread_local SQ    s("SELECT hidden FROM Folders WHERE id=?");
            return s.boolean(f.id);
        }
        
        Image               icon(Folder f)
        {
            static thread_local SQ    s("SELECT icon FROM Folders WHERE id=? LIMIT 1");
            return s.as<Image>(f.id);
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
        
        std::string             key(Folder f) 
        {
            static thread_local SQ    s("SELECT k FROM Folders WHERE id=?");
            return s.str(f.id);
        }

        std::string             label(Folder f)
        {
            return key(f);
        }
        
        void    make_directory(Folder fo, const Root* rt)
        {
            if(!rt)
                return ;
            if(fo == top_folder())
                return ;
                
            make_path(rt->resolve(key(fo)));
        }

        std::string             name(Folder f)
        {
            static thread_local SQ    s("SELECT name FROM Folders WHERE id=?");
            return s.str(f.id);
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
        
        Folder              parent(Folder f)
        {
            static thread_local SQ    s("SELECT parent FROM Folders WHERE id=?");
            return s.as<Folder>(f.id);
        }
        
        bool                removed(Folder f)
        {
            static thread_local SQ    s("SELECT removed FROM Folders WHERE id=?");
            return s.boolean(f.id);
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
        
        size_t              roots_count(Folder f)
        {
            static thread_local SQ    s("SELECT COUNT(DISTINCT root) FROM Directories WHERE folder=?");
            return s.size(f.id);
        }

        std::string             skey(Folder f)
        {
            static thread_local SQ    s("SELECT sk FROM Folders WHERE id=?");
            return s.str(f.id);
        }
        
        std::string             skeyb(Folder f)
        {
            std::string     bk  = skey(f);
            return copy(base_key(bk));
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
    }

////////////////////////////////////////////////////////////////////////////////

    namespace html {
        //WebHtml&    operator<<(WebHtml&, Folder);

        WebHtml&    operator<<(WebHtml&h, Dev<Folder> v)
        {
            std::string n       = cdb::name(v.data);
            if(n.empty())
                n   = "(no-name)";
            if(v.data)
                h << "<a href=\"/dev/folder?id=" << v.data.id << "\">";
            h << "[" << v.data.id << "] " << n;
            if(v.data)
                h << "</a>";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, DevID<Folder> v)
        {
            if(v.data)
                h << "<a href=\"/dev/folder?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }

        void    dev_table(WebHtml& h, const std::vector<Folder>&folders)
        {
            auto _tab = h.table();
            h << "<tr><th><ID></th><th>key</th><th>Name</th><th>Documents</th><th>Children</th><th>Brief</th></tr>\n";
            for(Folder f : folders){
                auto i = cdb::info(f);
                h << "<tr><td>" << dev_id(f) << "</td><td>" << i.key
                  << "</td><td>" << i.name << "</td><td>" << cdb::child_documents_count(f) << "</td><td>" << cdb::child_folders_count(f) 
                  << "</td><td>" << i.brief << "</td</tr>\n";
            }
        }
    }

////////////////////////////////////////////////////////////////////////////////
//  FRAGMENT
////////////////////////////////////////////////////////////////////////////////

    namespace arg {

        Fragment fragment(std::string_view  arg_string, const Root* rt)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Fragment{};
                
            Document      doc = document_key(arg_string);
            if(doc)
                return cdb::first_fragment(doc, rt);

            return fragment_id(arg_string);
        }
        
        Fragment fragment(const WebContext&ctx, const Root* rt, bool *detected)
        {
            if(detected)
                *detected   = false;
            
            std::string k    = ctx.find_query("id");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return fragment_id(k);
            }
            
            k   = ctx.find_query("root");
            if(!k.empty())
                rt  = root(k);

            k       = ctx.find_query("key");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return fragment_key(k, rt);
            }
            
            k       = ctx.find_query("fragment");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return fragment(k, rt);
            }
            return Fragment{};
        }
        
        Fragment fragment(const WebContext&ctx, std::string_view arg_name, const Root* rt, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return fragment(arg_string, rt);
        }
        
        Fragment fragment(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, const Root* rt, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return fragment(arg_string, rt);
        }
        
        Fragment fragment_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_fragment(i))
                return Fragment{i};
            return Fragment{};
        }

        Fragment fragment_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return fragment_id(arg_string);
        }
        
        Fragment fragment_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return fragment_id(arg_string);
        }
        
        Fragment fragment_key(std::string_view arg_string, const Root* rt)
        {
            Document  doc  = document_key(arg_string);
            if(!doc)
                return Fragment();
            return cdb::first_fragment(doc, rt);
        }
        
        Fragment fragment_key(const WebContext&ctx, std::string_view arg_name, const Root* rt, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return fragment_key(arg_string, rt);
        }
        
        Fragment fragment_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, const Root* rt, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return fragment_key(arg_string, rt);
        }
    }

////////////////////////////////////////////////////////////////////////////////

    namespace cdb {
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
        
        size_t              all_fragments_count()
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Fragments");
            return s.size();
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


        std::string             base_key(Fragment f)
        {
            return base_key(document(f));
        }

    #if 0
        std::string                 child_key(Fragment);
    #endif
        
        
        std::vector<uint8_t>         data(Fragment);     // TODO

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
        
        Directory           directory(Fragment f)
        {
            static thread_local SQ    s("SELECT dir FROM Fragments WHERE id=?");
            return s.as<Directory>(f.id);
        }

        Document            document(Fragment f)
        {
            static thread_local SQ    s("SELECT document FROM Fragments WHERE id=?");
            return s.as<Document>(f.id);
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
        
        bool                exists(Fragment f)
        {
            return exists_fragment(f.id);
        }

        bool                exists_fragment(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Fragments WHERE id=? LIMIT 1");
            return s.present(i);
        }
        
        Folder              folder(Fragment f)
        {
            static thread_local SQ    s("SELECT folder FROM Fragments WHERE id=?");
            return s.as<Folder>(f.id);
        }

        std::vector<Folder>      folder_path(Fragment f)
        {
            return folder_path(document(f));
        }
        
        ByteArray           frag_bytes(Fragment f, cdb_options_t opts)
        {
            std::filesystem::path   p = path(f);
            Fragment::Lock  lk;
            if(!(opts & DONT_LOCK)){
                lk = Fragment::Lock::read(f);
                if(!lk){
                    yWarning() << "Unable to get read lock on fragment: " << p;
                    return ByteArray();
                }
            }
                
            return file_bytes(p);
        }
        
        ByteArray           frag_bytes(std::string_view k, cdb_options_t opts)
        {
            return frag_bytes(fragment(document(k)), opts);
        }
        
        ByteArray           frag_bytes(std::string_view k, DataRole dr, cdb_options_t opts)
        {
            return frag_bytes(fragment(document(k),dr), opts);
        }
        
        
        std::string              frag_string(Fragment f, cdb_options_t opts)
        {
            std::filesystem::path   p = path(f);
            Fragment::Lock  lk;
            if(!(opts & DONT_LOCK)){
                lk = Fragment::Lock::read(f);
                if(!lk){
                    yWarning() << "Unable to get read lock on fragment: " << p;
                    return std::string();
                }
            }
                
            return file_string(p);
        }
        
        std::string              frag_string(std::string_view k, cdb_options_t opts)
        {
            return frag_string(fragment(document(k)), opts);
        }
        
        std::string              frag_string(std::string_view k, DataRole dr, cdb_options_t opts)
        {
            return frag_string(fragment(document(k),dr), opts);
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
        
        
        std::vector<Fragment>    fragments(std::string_view k, Sorted sorted)
        {
            return fragments(document(k),sorted);
        }
        
        std::vector<Fragment>    fragments(std::string_view k, DataRole dr, Sorted sorted)
        {
            return fragments(document(k), dr, sorted);
        }

        bool                hidden(Fragment f)
        {
            static thread_local SQ    s("SELECT hidden FROM Fragments WHERE id=?");
            return s.boolean(f.id);
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
        
        std::string             key(Fragment f)
        {
            return key(document(f));
        }
        
        std::string             label(Fragment f)
        {
            return path(f).string();
        }

        uint64_t                modified(Fragment f)
        {
            static thread_local SQ    s("SELECT modified FROM Fragments WHERE id=?");
            return s.u64(f.id);
        }
        
        std::string             name(Fragment f)
        {
            static thread_local SQ    s("SELECT name FROM Fragments WHERE id=?");
            return s.str(f.id);
        }
        
        Directory           parent(Fragment f)
        {
            return directory(f);
        }

        std::filesystem::path   path(Fragment f)
        {
            static thread_local SQ    s("SELECT path FROM Fragments WHERE id=?");
            return s.path(f.id);
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

        const Root*         root(Fragment f)
        {
            static thread_local SQ    s("SELECT root FROM Fragments WHERE id=?");
            auto s_af   = s.af();
            s.bind(1, f.id);
            if(s.step() == SqlQuery::Row)
                return wksp::root(s.v_uint64(1));
            return nullptr;
        }

        size_t              size(Fragment f)
        {
            static thread_local SQ    s("SELECT bytes FROM Fragments WHERE id=?");
            return s.size(f.id);
        }
        
        std::string             skey(Fragment f)
        {
            static thread_local SQ    s("SELECT name FROM Fragments WHERE id=?");
            return s.str(f.id);
        }
        
        std::string             skeyb(Fragment f)
        {
            std::string     bk  = skey(f);
            return copy(base_key(bk));
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
    }

////////////////////////////////////////////////////////////////////////////////

    namespace html {
        //WebHtml&    operator<<(WebHtml&, Fragment);

        WebHtml&    operator<<(WebHtml&h, Dev<Fragment> v)
        {
            std::string n       = cdb::name(v.data);
            if(n.empty())
                n   = "(no-name)";
            if(v.data)
                h << "<a href=\"/dev/fragment?id=" << v.data.id << "\">";
            h << "[" << v.data.id << "] " << n;
            if(v.data)
                h << "</a>";
            return h;
        }

        WebHtml&    operator<<(WebHtml&h, DevID<Fragment> v)
        {
            if(v.data)
                h << "<a href=\"/dev/fragment?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }

        void    dev_table(WebHtml& h, const std::vector<Fragment>& fragments)
        {
            auto _tab = h.table();
            h << "<tr><th>ID</th><th>Name</th><th>Size</th><th>Path</th>\n";
            for(Fragment f : fragments){
                auto i = cdb::info(f);
                h << "<tr><td>" << dev_id(f) << "</td><td>" << i.name 
                    << "</td><td>" << i.size << "</td><td>" << i.path << "</td></tr>\n";
            }
        }

    }

////////////////////////////////////////////////////////////////////////////////
//  ROOT
////////////////////////////////////////////////////////////////////////////////

    namespace arg {

        const Root* root(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return nullptr;
                
            const Root* t   = wksp::root( arg_string);
            if(t)
                return t;
            return wksp::root(to_uint64( arg_string).value);
        }
        
        const Root* root(const WebContext&ctx, bool *detected)
        {
            if(detected)
                *detected   = false;
                
            std::string    k    = ctx.find_query("id");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return root_id(k);
            }
            
            k       = ctx.find_query("key");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return root_key(k);
            }
            
            k       = ctx.find_query("root");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return root(k);
            }
            return nullptr;
        }
        
        const Root* root(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return root(arg_string);
        }
        
        const Root* root(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return root(arg_string);
        }

        const Root* root_id(std::string_view arg_string)
        {
            return wksp::root(to_uint64(arg_string).value);
        }

        const Root* root_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return root_id(arg_string);
        }
        
        const Root* root_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return root_id(arg_string);
        }
        
        const Root* root_key(std::string_view arg_string)
        {
            return wksp::root(trimmed(arg_string));
        }
        
        const Root* root_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return root_key(arg_string);
        }
        
        const Root* root_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return root_key(arg_string);
        }
    }

////////////////////////////////////////////////////////////////////////////////


    namespace cdb {
        
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
        
        size_t              all_directories_count(const Root*rt)
        {
            if(!rt)
                return 0;

            static thread_local SQ    s("SELECT COUNT(1) FROM Directories WHERE root=?");
            return s.size(rt->id);
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
        
        size_t              all_fragments_count(const Root*rt)
        {
            if(!rt)
                return 0;
            static thread_local SQ s("SELECT COUNT(1) FROM Fragments WHERE root=?");
            return s.size(rt->id);
        }

        size_t                      all_roots_count()
        {
            return wksp::roots().size();
        }
        
        std::vector<DirOrFrag>   children(const Root* rt, Sorted sorted)
        {
            return children(directory(rt), sorted);
        }
        
        std::vector<uint8_t>         data(Fragment);     // TODO
        
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

        Directory           directory(const Root*rt)
        {
            if(!rt)
                return Directory();
            return directory(rt->path);
        }
        
        namespace {
            std::vector<Directory>   directories_unsorted(const Root*rt)
            {
                if(!rt)
                    return std::vector<Directory>();
                    
                static thread_local SQ    s("SELECT id FROM Directories WHERE parent=0 AND root=?");
                return s.vec<Directory>(rt->id);
            }

            std::vector<Directory>   directories_sorted(const Root*rt)
            {
                if(!rt)
                    return std::vector<Directory>();
                    
                static thread_local SQ    s("SELECT id FROM Directories WHERE parent=0 AND root=? ORDER BY NAME");
                return s.vec<Directory>(rt->id);
            }
        }
        
        std::vector<Directory>   directories(const Root*rt, Sorted sorted)
        {
            return sorted ? directories_sorted(rt) : directories_unsorted(rt);
        }

        size_t              directories_count(const Root* rt)
        {
            if(!rt)
                return 0;
            static thread_local SQ    s("SELECT COUNT(1) FROM Directories WHERE parent=0 AND root=?");
            return s.size(rt->id);
        }
        
        bool                exists(const Root*rt, std::string_view z)
        {
            if(!rt)
                return false;
            return std::filesystem::exists(rt -> resolve(z));
        }
        

        Fragment            fragment(const Root*rt, std::string_view z)
        {
            if(!rt)
                return Fragment{};
            return fragment(rt->resolve(z));
        }

        
        std::vector<Fragment>    fragments(const Root*rt, Sorted sorted)
        {
            return child_fragments(directory(rt), sorted);
        }

        std::string             key(const Root*rt)
        {
            if(!rt)
                return std::string();
            return rt->key;
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
    }

////////////////////////////////////////////////////////////////////////////////

    namespace html {

        WebHtml&    operator<<(WebHtml&h, Dev<const Root*> v)
        {
            if(!v.data){
                h << "(null root)";
                return h;
            }
            
            h << "<a href=\"/dev/root?id=" << v.data->id << "\">[" << v.data->id << "] " << v.data->key << "</a>";
            return h;
        }

        WebHtml&    operator<<(WebHtml& h, DevID<const Root*> v)
        {
            if(!v.data){
                h << "(null root)";
                return h;
            }
            
            h << "<a href=\"/dev/root?id=" << v.data->id << "\">" << v.data->id << "</a>";
            return h;
        }

        void    dev_table(WebHtml&h, const std::vector<const Root*>& roots)
        {
            auto _tab = h.table();
            h << "<tr><th><ID></th><th>Key</th><th>Name</th><th>Path</th></tr>\n";
            for(const Root* r : roots){
                if(!r)
                    continue;
                h << "<tr><td>" << dev_id(r) <<  "</td><td>" << r->key << "</td><td>" << r->name << "</td><td>" << r->path << "</td></tr>\n";
            }
        }

    }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}

