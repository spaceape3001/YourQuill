////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/directory/DirectoryJson.hpp>
#include <mithril/directory/DirectorySearch.hpp>

namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

        json p_api_directory(WebContext& ctx)
        {
            Directory    v   = arg::directory(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            return json_(v);
        }
        
        json p_api_directory_key(WebContext& ctx)
        {
            Directory    v   = arg::directory(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            json j{
                { "key", cdb::key(v) }
            };
            return j;
        }
        
        json p_api_directory_path(WebContext& ctx)
        {
            Directory    v   = arg::directory(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            json j{
                { "path", cdb::path(v) }
            };
            return j;
        }
        

        json p_api_directories(WebContext& ctx)
        {
            DirectoryVector  ret = search(ctx, DIRECTORY);
            return json{
                { "directories", json_(ret) }
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

        void    p_dev_directories(WebHtml& h)
        {
            h.title("All Directories");
            dev_table(h, all_directories(Sorted::YES));
        }
        
        void    p_dev_directory(WebHtml& h)
        {
            Directory   x   = directory(h);
            if(!x)
                throw HttpStatus::BadArgument;
            Directory   p   = parent(x);
            Folder      f   = folder(x);
            const RootDir* rt  = root_dir(x);
            
            dev_title(h, x);
            auto ta = h.table();
            h.kvrow("ID") << x.id;
            h.kvrow("Key") << key(x);
            h.kvrow("Folder") << dev(f);
            h.kvrow("Hidden") << hidden(x);
            if(p)
                h.kvrow("Parent") << dev(p);
            if(rt)
                h.kvrow("RootDir") << dev(rt);
        }
        
        void    p_dev_directory_fragments(WebHtml& h)
        {
            Directory   x   = directory(h);
            if(!x)
                throw HttpStatus::BadArgument;
            
            dev_title(h, x, "Fragments");
            dev_table(h, child_fragments(x, Sorted::YES));
        }

        void    p_dev_directory_subdirs(WebHtml& h)
        {
            Directory   x   = directory(h);
            if(!x)
                throw HttpStatus::BadArgument;
            
            dev_title(h, x, "Subdirectories");
            dev_table(h, child_directories(x, Sorted::YES));
        }
    
    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_directory_pages()
        {
            reg_webpage<p_api_directory>("/api/directory").argument("ID", "Directory ID");
            reg_webpage<p_api_directory_key>("/api/directory/key").argument("ID", "Directory ID");
            reg_webpage<p_api_directory_path>("/api/directory/path").argument("ID", "Directory ID");
            reg_webpage<p_api_directories>("/api/directories")
                .argument("folder", "Specify folder")
                .argument("parent", "Specify parent")
                .argument("root", "Specify root directory")
                .description("Lists directories according to search criteria (or ALL directories w/o search terms)");

            reg_webpage<p_dev_directories>("/dev/directories");
            reg_webgroup({
                reg_webpage<p_dev_directory>("/dev/directory").argument("id", "Directory ID").label("Info"),
                reg_webpage<p_dev_directory_fragments>("/dev/directory/fragments").argument("id", "Directory ID").label("Frags"),
                reg_webpage<p_dev_directory_subdirs>("/dev/directory/subdirs").argument("id", "Directory ID").label("Subdirs")
            });
        }
}
