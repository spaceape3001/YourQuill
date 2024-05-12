////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/novel/NovelJson.hpp>
#include <mithril/novel/NovelSearch.hpp>


namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------
    
    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

        json p_api_novel(WebContext& ctx)
        {
            Novel    v   = arg::novel(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            return json_(v);
        }
        
        json p_api_novel_key(WebContext& ctx)
        {
            Novel    v   = arg::novel(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            json j{
                { "key", cdb::key(v) }
            };
            return j;
        }
        
        json p_api_novels(WebContext& ctx)
        {
            NovelVector  ret = search(ctx, NOVEL);
            return json{
                { "novels", json_(ret) }
            };
        }
        
    //  -----------------------------------------------------------------------
    //      PAGES
    //  -----------------------------------------------------------------------
        
        #if 0
        std::string_view title_of(const Context& ctx)
        {
            if(!ctx.title.empty())
                return ctx.title;
            if(ctx.root_dir){
                if(!ctx.root_dir->name.empty())
                    return ctx.root_dir->name;
                if(!ctx.root_dir->key.empty())
                    return ctx.root_dir->key;
            }
            return std::string_view();
        }
        #endif


    //  -----------------------------------------------------------------------
    //      ADMIN
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      DEV
    //  -----------------------------------------------------------------------


        void p_dev_novel(WebHtml&h)
        {
            Novel    x   = novel(h);
            if(!x)
                throw HttpStatus::BadArgument;
            
            auto i = nki(x);

            dev_title(h, x);
            auto t = h.table();
            h.kvrow("ID") << x.id;
            h.kvrow("Key") << i.key;
            h.kvrow("Title") << i.name;
            //h.kvrow("Brief") << i.brief;
            h.kvrow("Fragment") << dev(fragment(x));
            //h.kvrow("Atom") << dev(i.atom);
       }
        
        //  we'll do more (later)

        void    p_dev_novels(WebHtml&h)
        {
            h.title("All Novels");
            dev_table(h, all_novels(Sorted::YES));
        }

    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_novel_pages()
        {
            reg_webpage<p_api_novel>("/api/novel").argument("ID", "Novel ID");
            reg_webpage<p_api_novel_key>("/api/novel/key").argument("ID", "Novel ID");
            reg_webpage<p_api_novels>("/api/novels");

            reg_webgroup({
                reg_webpage<p_dev_novel>("/dev/novel").argument("id", "Novel ID").label("Info")
            });
            reg_webpage<p_dev_novels>("/dev/novels");
        }
}
