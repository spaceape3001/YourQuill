////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/folder/FolderJson.hpp>
#include <yq/mithril/folder/FolderSearch.hpp>

namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

        json p_api_folder(WebContext& ctx)
        {
            Folder    v   = arg::folder(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            return json_(v);
        }
        
        json p_api_folder_key(WebContext& ctx)
        {
            Folder    v   = arg::folder(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            json j{
                { "key", cdb::key(v) }
            };
            return j;
        }
        
        json p_api_folders(WebContext& ctx)
        {
            FolderVector  ret = search(ctx, FOLDER);
            return json{
                { "folders", json_(ret) }
            };
        }

    //  -----------------------------------------------------------------------
    //      PAGES
    //  -----------------------------------------------------------------------
    
    //  -----------------------------------------------------------------------
    //      ADMIN
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      DEV
    //  -----------------------------------------------------------------------
    
        void    p_dev_folder(WebHtml&h)
        {
            bool    detected;
            Folder  x   = folder(h, &detected);
            if(!x){
                if(detected)
                    throw HttpStatus::BadArgument;
                //  redirect...once we get a streaming API to it
                x       = top_folder();
            }

            
            auto i = info(x);
            dev_title(h, x);
            auto t = h.table();
            h.kvrow("ID") << x.id;
            h.kvrow("Name") << i.name;
            h.kvrow("Key") << "'" << i.key << "'";
            h.kvrow("Parent") << dev(i.parent);
            h.kvrow("Hidden") << i.hidden;
            h.kvrow("Removed") << i.removed;
            h.kvrow("Sub-Key") << i.skey;
            h.kvrow("Brief") << i.brief;
            h.kvrow("Folders") << child_folders_count(x);
            h.kvrow("Documents") << child_documents_count(x);
        }
        
        void    p_dev_folder_directories(WebHtml&h)
        {
            Folder  x   = folder(h);
            if(!x)
                throw HttpStatus::BadArgument;
            dev_title(h, x, "Directories");
            dev_table(h, directories(x));
        }

        void    p_dev_folder_documents(WebHtml&h)
        {
            Folder  x   = folder(h);
            if(!x)
                throw HttpStatus::BadArgument;
            
            dev_title(h, x, "Documents");
            dev_table(h, child_documents(x));
        }

        void    p_dev_folder_fragments(WebHtml&h)
        {
            Folder  x   = folder(h);
            if(!x)
                throw HttpStatus::BadArgument;
            
            dev_title(h, x, "Fragments");
            dev_table(h, child_fragments(x));
        }

        void    p_dev_folder_subs(WebHtml&h)
        {
            Folder  x   = folder(h);
            if(!x)
                throw HttpStatus::BadArgument;
            
            dev_title(h, x, "Sub-Folders");
            dev_table(h, child_folders(x));
        }

        void    p_dev_folders(WebHtml& h)
        {
            h.title("Listing of Folders");
            dev_table(h, all_folders(Sorted::YES));
        }
        
    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_folder_pages()
        {
            reg_webpage<p_api_folder>("/api/folder").argument("ID", "Folder ID");
            reg_webpage<p_api_folder_key>("/api/folder/key").argument("ID", "Folder ID");
            reg_webpage<p_api_folders>("/api/folders")
                .argument("parent", "Specify owning folder")
            ;

            reg_webgroup({
                reg_webpage<p_dev_folder>("/dev/folder").label("Info"),
                reg_webpage<p_dev_folder_directories>("/dev/folder/dirs").label("Dirs"),
                reg_webpage<p_dev_folder_documents>("/dev/folder/docs").label("Docs"),
                reg_webpage<p_dev_folder_fragments>("/dev/folder/frags").label("Frags"),
                reg_webpage<p_dev_folder_subs>("/dev/folder/subs").label("Subs")
            });
            reg_webpage<p_dev_folders>("/dev/folders");
        }
}
