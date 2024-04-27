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
        json    p_api_wksp(WebContext&ctx)
        {
            json    ret{
                { "author", wksp::author() },
                { "abbreviation", wksp::abbreviation() },
                { "bkcolor", gBkColor },
                { "copyright", wksp::copyright().text },
                { "c_stance", wksp::copyright().stance.key() },
                { "c_from", wksp::copyright().from },
                { "c_to", wksp::copyright().to },
                { "color", gTextColor },
                { "name", wksp::name() }
            };
            if(ctx.is_local()){
                ret["quill"] = wksp::quill_file().string();
                ret["cache"] = wksp::cache().string();
            }
            return ret;
        }

        json    p_api_wksp_quill(WebContext&ctx)
        {
            json    ret;
            if(ctx.is_local()){
                ret["quill"] = wksp::quill_file().string();
            }
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

        void  p_dev_wksp(WebHtml& h)
        {
            h.title() << "Workspace '" << wksp::name() << "'";
            
            auto t = h.table();
            h.kvrow("Name") << wksp::name();
            h.kvrow("Author") << wksp::author();
            h.kvrow("Abbreviation") << wksp::abbreviation();
            h.kvrow("Cache") << wksp::cache();
            h.kvrow("Color (Background)") << gBkColor.get();
            h.kvrow("Color (Text)") << gTextColor.get();
            //h.kvrow("Config") << dev(config_folder());
            h.kvrow("Copyright") << wksp::copyright().text;
            h.kvrow("Key") << wksp::quill_key();
            h.kvrow("Host") << wksp::host();
            h.kvrow("Port") << wksp::port();
            h.kvrow("Read Timeout") << wksp::read_timeout();
            h.kvrow("Start") << wksp::start();
            h.kvrow("Templates") << join(wksp::templates(), ", ");
        }

        void    p_dev_wksp_roots(WebHtml&h)
        {
            h.title("Workspace Roots");
            dev_table(h, wksp::root_dirs());
        }

    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void    reg_wksp_pages()
        {
            reg_webpage<p_api_wksp>("/api/wksp"sv); 
            reg_webpage<p_api_wksp_quill>("/api/wksp/quill"sv); 

            reg_webgroup({
                reg_webpage<p_dev_wksp>("/dev/wksp").label("Info"),
                reg_webpage<p_dev_wksp_roots>("/dev/wksp/roots").label("Roots")
            });
        }
}
