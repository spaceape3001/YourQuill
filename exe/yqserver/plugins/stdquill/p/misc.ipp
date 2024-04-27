////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

        json    p_api_easter(WebContext& ctx)
        {
            std::string     arg_string = ctx.find_query("year");
            if(arg_string.empty())
                throw HttpStatus::BadArgument;
            auto    x   = to_int16(arg_string);
            if(!x)
                throw HttpStatus::BadArgument;
                
            int16_t y   = *x;
            Date    d   = easter(y);
            
            json    ret{
                { "month", to_string(d.month) },
                { "day", to_string(d.day) }
            };
            return ret;
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

        void    p_dev_hello(WebHtml& out)
        {
            out.title("HELLO WORLD!");
            
            auto _t     = out.table();
            
            out.kvrow("Method") << out.context().method.key() << "\n";
            out.kvrow("URL") << out.context().url << "\n";
            out.kvrow("Client HTTP") << out.context().version << "</td></tr>\n";
            for(auto& hv : out.context().rx_headers)
                out.kvrow(hv.first) << hv.second;
        }
        

    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_misc_pages()
        {
            reg_webpage<p_api_easter>("/api/easter"sv);

            reg_webpage<p_dev_hello>("/dev/hello");
        }
}
