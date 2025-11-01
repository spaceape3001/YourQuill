////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/doodle/DoodleJson.hpp>
#include <yq/mithril/doodle/DoodleSearch.hpp>

namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------
    
    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

        json p_api_doodle(WebContext& ctx)
        {
            Doodle    v   = arg::doodle(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            return json_(v);
        }
        
        json p_api_doodle_key(WebContext& ctx)
        {
            Doodle    v   = arg::doodle(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            json j{
                { "key", cdb::key(v) }
            };
            return j;
        }
        
        json p_api_doodles(WebContext& ctx)
        {
            DoodleVector  ret = search(ctx, DOODLE);
            return json{
                { "doodles", json_(ret) }
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

        void    p_dev_doodle(WebHtml& h)
        {
            Doodle   x = doodle(h);
            if(!x)
                throw HttpStatus::BadArgument;
            auto i = cdb::nki(x);
            dev_title(h, x);
            auto ta = h.table();
            h.kvrow("ID") << x.id;
            h.kvrow("Title") << i.name;
            h.kvrow("Key") << i.key;
            h.kvrow("Fragment") << dev(cdb::fragment(x));
        }

        void    p_dev_doodles(WebHtml& out)
        {
            out.title("All Doodles");
            dev_table(out, all_doodles());
        }

    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_doodle_pages()
        {
            reg_webpage<p_api_doodle>("/api/doodle").argument("ID", "Doodle ID");
            reg_webpage<p_api_doodle_key>("/api/doodle/key").argument("ID", "Doodle ID");
            reg_webpage<p_api_doodles>("/api/doodles");

            reg_webpage<p_dev_doodle>("/dev/doodle").local().argument("id", "Doodle id (number)");
            reg_webpage<p_dev_doodles>("/dev/doodles");
        }
}
