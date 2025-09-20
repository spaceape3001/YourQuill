////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/thing/ThingJson.hpp>
#include <mithril/thing/ThingSearch.hpp>

namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

        json p_api_thing(WebContext& ctx)
        {
            Thing    v   = arg::thing(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            return json_(v);
        }
        
        json p_api_thing_key(WebContext& ctx)
        {
            Thing    v   = arg::thing(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            json j{
                { "key", cdb::key(v) }
            };
            return j;
        }
        
        json p_api_things(WebContext& ctx)
        {
            ThingVector  ret = search(ctx, THING);
            return json{
                { "things", json_(ret) }
            };
        }

    //  -----------------------------------------------------------------------
    //      PAGES
    //  -----------------------------------------------------------------------

        void p_things(WebHtml& h)
        {
            //  Search parameters go here... execute the search
            std::vector<Thing>  things   = all_things(Sorted::YES);
            
            
            //  Form the title... 
            h.title() << "Things";
            
            h << "<p><i>(Search Bar will go here)</i></p>\n";
            
            //  And results
            
            
            
            dev_table(h, things);
        }    
        
    //  -----------------------------------------------------------------------
    //      ADMIN
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      DEV
    //  -----------------------------------------------------------------------

        void    p_dev_things(WebHtml& h)
        {
            h.title() << "All Things";
            dev_table(h, all_things(Sorted::YES));
        }
        
    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_thing_pages()
        {
            reg_webpage<p_api_thing>("/api/thing").argument("ID", "Thing ID");
            reg_webpage<p_api_thing_key>("/api/thing/key").argument("ID", "Thing ID");
            reg_webpage<p_api_things>("/api/things");

            reg_webtemplate("/things", wksp::shared("www/things.ht"sv));
            
            reg_webpage<p_dev_things>("/dev/things");
        }
}
