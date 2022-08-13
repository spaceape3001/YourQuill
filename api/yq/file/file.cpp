////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <kernel/file/DirectoryCDB.hpp>
#include "DirectoryArg.hpp"
#include "DirectoryHtml.hpp"
#include "DirectoryPost.hpp"

#include <kernel/file/DocumentCDB.hpp>
#include "DocumentArg.hpp"
#include "DocumentHtml.hpp"
#include "DocumentPost.hpp"

#include <kernel/file/FolderCDB.hpp>
#include "FolderArg.hpp"
#include "FolderHtml.hpp"
#include "FolderPost.hpp"

#include <kernel/file/FragmentCDB.hpp>
#include "FragmentArg.hpp"
#include "FragmentHtml.hpp"
#include "FragmentPost.hpp"

#include <kernel/file/Root.hpp>
#include <kernel/file/RootCDB.hpp>
#include "RootArg.hpp"
#include "RootHtml.hpp"
#include "RootPost.hpp"

#include <basic/CollectionUtils.hpp>
#include <kernel/db/NKI.hpp>
#include <kernel/image/ImageCDB.hpp>
#include <basic/DirUtils.hpp>
#include <basic/FileUtils.hpp>
#include <basic/StreamOps.hpp>
#include <basic/TextUtils.hpp>
#include <basic/HttpParser.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>
#include <kernel/wksp/Workspace.hpp>


namespace yq {
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

    namespace post {

        Directory directory(WebContext&ctx, bool *detected)
        {
            ctx.decode_post();
            
            if(detected)
                *detected   = false;
                
            std::string    k    = ctx.rx_post.first("directory");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return arg::directory_id(k);
            }
            return Directory();
        }
        
        Directory directory(WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_name));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::directory_id(arg_string);
        }
        
        Directory directory(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_names));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::directory_id(arg_string);
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

        WebHtml&    operator<<(WebHtml&h, const Icon<Document>& v)
        {
            std::string a   = v.alt;
            if(a.empty())
                a   = cdb::key(v.src);
            
            Image   i   = cdb::icon(v.src);
            if(i){
                h << cdb::thumbnail(i, a);
            } else {
                Icon<std::string>   fwd;
                fwd.src = "/img/document.svg";
                fwd.style = v.style;
                fwd.alt = std::move(a);
                h << fwd;
            }
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

    namespace post {

        Document document(WebContext&ctx, bool *detected)
        {
            ctx.decode_post();
            
            if(detected)
                *detected   = false;
                
            std::string    k    = ctx.rx_post.first("document");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return arg::document_id(k);
            }
            return Document();
        }
        
        Document document(WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_name));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::document_id(arg_string);
        }
        
        Document document(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_names));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::document_id(arg_string);
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

        WebHtml&    operator<<(WebHtml&h, const Icon<Folder>&v)
        {
            std::string a   = v.alt;
            if(a.empty())
                a   = cdb::key(v.src);
            
            Image   i   = cdb::icon(v.src);
            if(i){
                h << cdb::thumbnail(i, a);
            } else {
                Icon<std::string>   fwd;
                fwd.src = "/img/folder.svg";
                fwd.style = v.style;
                fwd.alt = std::move(a);
                h << fwd;
            }
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

    namespace post {

        Folder folder(WebContext&ctx, bool *detected)
        {
            ctx.decode_post();
            
            if(detected)
                *detected   = false;
                
            std::string    k    = ctx.rx_post.first("folder");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return arg::folder_id(k);
            }
            return Folder();
        }
        
        Folder folder(WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_name));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::folder_id(arg_string);
        }
        
        Folder folder(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_names));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::folder_id(arg_string);
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

    namespace post {

        Fragment fragment(WebContext&ctx, bool *detected)
        {
            ctx.decode_post();
            
            if(detected)
                *detected   = false;
                
            std::string    k    = ctx.rx_post.first("fragment");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return arg::fragment_id(k);
            }
            return Fragment();
        }
        
        Fragment fragment(WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_name));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::fragment_id(arg_string);
        }
        
        Fragment fragment(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_names));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::fragment_id(arg_string);
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
        

        WebHtml&    operator<<(WebHtml&h, const ControlRoot&v)
        {
            auto& ctx   = h.context();
            
            auto qr = ctx.strip_query("root");
            const Root* rt  = yq::arg::root(qr.value);
            
            Url newurl      = copy(ctx.url);
            newurl.query    = qr.rest;
            
            h << "<table align=\"right\" class=\"roots\">\n";
            h << "<tr><td class=\"roots\">";
            if(rt)
                h << "<a href=\"" << newurl << "\">";
            h << "(Fused)";
            if(rt)
                h << "</a>";
            h << "</td></tr><tr><td class=\"roots\"><hr width=\"10%\"></td></tr>\n";
            
            for(const Root* r : wksp::roots()){
                newurl.query    = qr.rest;
                if(newurl.query.empty())
                    newurl.query   = "?root=";
                else
                    newurl.query  += "&root=";
                newurl.query += r->key;
                h << "<tr><td class=\"roots\">";
                if(r != rt)
                    h << "<a href=\"" << newurl << "\">";
                h << r -> name;
                if(r != rt)
                    h << "</a>";
                h  << "</td></tr>\n";
            }
            h << "</table>\n";
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, const IRoot&v)
        {
            const Root* re = v.root;
            if(!re)
                re  = h.context().def_root(v.role);
            if(!v.msg.empty()){
                html_escape_write(h, v.msg);
                h << "<br>";
            }
                
            for(const Root* rt : wksp::roots()){
                h << "<input type=\"radio\" name=\"root\" id=\"" << rt->key << "\" value=\"" << rt->id << "\"";
                if(!rt->is_good(v.role, v.access))
                    h << " disabled=\"true\"";
                h   << "><label for=\"" << rt->key << "\">";
                if(rt == re)
                    h << "<b><u>";
                h << "[" << rt->key << "] " << rt->name;
                if(rt == re)
                    h << "</u></b>";
                h  << "</label><br>\n";
            }
            return h;
        }

        void    dev_table(WebHtml&h, const std::vector<const Root*>& roots)
        {
            auto _tab = h.table();
            h << "<tr><th>ID</th><th>Key</th><th>Name</th><th>Path</th></tr>\n";
            for(const Root* r : roots){
                if(!r)
                    continue;
                h << "<tr><td>" << dev_id(r) <<  "</td><td>" << r->key << "</td><td>" << r->name << "</td><td>" << r->path << "</td></tr>\n";
            }
        }

    }

////////////////////////////////////////////////////////////////////////////////


    namespace post {

        const Root* root(WebContext&ctx, bool *detected)
        {
            ctx.decode_post();
            
            if(detected)
                *detected   = false;
                
            std::string    k    = ctx.rx_post.first("root");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return arg::root_id(k);
            }
            return nullptr;
        }
        
        const Root* root(WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_name));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::root_id(arg_string);
        }
        
        const Root* root(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_names));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::root_id(arg_string);
        }
    }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}

