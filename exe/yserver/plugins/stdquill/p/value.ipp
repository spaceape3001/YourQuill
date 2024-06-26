////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/value/ValueJson.hpp>
#include <mithril/value/ValueSearch.hpp>


namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

        json p_api_value(WebContext& ctx)
        {
            Value    v   = arg::value(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            return json_(v);
        }
        
        
        json p_api_values(WebContext& ctx)
        {
            ValueVector  ret = search(ctx, VALUE);
            return json{
                { "values", json_(ret) }
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

    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_value_pages()
        {
            reg_webpage<p_api_value>("/api/value").argument("ID", "Value ID");
            reg_webpage<p_api_values>("/api/values");
        }
}
