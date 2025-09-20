////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/place/PlaceJson.hpp>
#include <mithril/place/PlaceSearch.hpp>

namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------


        json p_api_place(WebContext& ctx)
        {
            Place    v   = arg::place(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            return json_(v);
        }
        
        json p_api_place_key(WebContext& ctx)
        {
            Place    v   = arg::place(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            json j{
                { "key", cdb::key(v) }
            };
            return j;
        }
        
        json p_api_places(WebContext& ctx)
        {
            PlaceVector  ret = search(ctx, PLACE);
            return json{
                { "places", json_(ret) }
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

        void    p_dev_places(WebHtml& h)
        {
            h.title() << "All Places";
            dev_table(h, all_places(Sorted::YES));
        }

    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_place_pages()
        {
            reg_webpage<p_api_place>("/api/place").argument("ID", "Place ID");
            reg_webpage<p_api_place_key>("/api/place/key").argument("ID", "Place ID");
            reg_webpage<p_api_places>("/api/places");

            reg_webtemplate("/places", wksp::shared("www/places.ht"sv));
            
            reg_webpage<p_dev_places>("/dev/places");
        }
}
