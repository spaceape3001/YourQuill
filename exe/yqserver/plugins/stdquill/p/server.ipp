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

    //  -----------------------------------------------------------------------
    //      PAGES
    //  -----------------------------------------------------------------------
    
    //  -----------------------------------------------------------------------
    //      ADMIN
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      DEV
    //  -----------------------------------------------------------------------

    
        void    p_dev_echo(WebHtml& h)
        {
            auto& ctx   = h.context();
            ctx.decode_query();
            ctx.decode_post();

            h.title("Developer's Echo");
            
            bool    isPost  = ctx.method == hPost;
            
            h.h2("Environment");
            {
                auto ta = h.table();
                h.kvrow("Session ID") << ctx.session.ssid;
                h.kvrow("User") << ctx.session.username;
                h.kvrow("Time") << ctx.timestamp;
                h.kvrow("Path") << ctx.url.path;
                h.kvrow("Method") << ctx.method;
                h.kvrow( "From") << ctx.remote_addr.to_string() << ":" << ctx.remote_port;
            }
            
            h.h2("URL Query");
            h << ctx.rx_query;

            if(isPost){
                h.h2("Post Parameters");
                std::string     url = ctx.rx_post.first("url");
                if(!url.empty()){
                    h << "<form action=\"" << url << "\" method=\"post\">\n";
                    for(auto& i : ctx.rx_post_raw)
                        h << "<input type=\"hidden\" name=\"" << i.first << "\" id=\"" << i.first << "\" value=\"" << i.second << "\">\n";
                    h << "<input type=\"submit\" value=\"Forward\">\n"
                      << "</form>\n";
                }
                
                h << ctx.rx_post;
            }
            
            h.h2("Headers");
            h << ctx.rx_headers;
            
            if(!ctx.rx_body.empty()){
                h.h2("Body");
                h << "<pre>\n";
                html_escape_write(h, ctx.rx_body);
                h << "</pre>\n";
            }
        }

        void p_dev_server(WebHtml&h)
        {
            h.title("Server Information");
            auto ta  = h.table();
            
            h.kvrow("Abbreviation") << wksp::abbreviation();
            h.kvrow("Author") << wksp::author();
            h.kvrow("Auxillary Ports") << join(wksp::aux_ports(), ", ");
            h.kvrow("Cache") << wksp::cache();
            h.kvrow("Dot") << wksp::dot();
            h.kvrow("GIT") << wksp::git();
            h.kvrow("Home") << wksp::home();
            h.kvrow("Host") << wksp::host();
            h.kvrow("Local User") << wksp::local_user();
            h.kvrow("Log Directory") << wksp::log_dir();
            h.kvrow("Markdown") << wksp::markdown();
            h.kvrow("Name") << wksp::name();
            h.kvrow("Perl") << wksp::perl();
            h.kvrow("Port") << wksp::port();
            h.kvrow("Quill Key") << wksp::quill_key();
            h.kvrow("Quill Path") << wksp::quill_file();
            h.kvrow("Read Timeout") << wksp::read_timeout();
            
            {
                auto r = h.kvrow("Roots");
                r << "<OL>";
                for(const RootDir* rt : wksp::root_dirs())
                    r << "<LI>[" << rt -> key << "]: " << rt->path;
                r << "</OL>";
            }
            
            h.kvrow("Smartypants") << wksp::smartypants();
            h.kvrow("Start") << wksp::start();
            
            h.kvrow("Temp Path") << wksp::temp_dir();
            
            {
                auto r = h.kvrow("Templates");
                r << "<OL>";
                for(const std::string& t : wksp::templates())
                    r << "<LI>" << t;
                r << "</OL>";
            }
        }


        void    p_dev_webdirs(WebHtml&h)
        {
            h.title("All WebDirs");
            dev_table(h, web::directory_map());
        }

        void p_dev_webexts(WebHtml& h)
        {
            h.title("All WebExts");
            dev_table(h, web::extension_map());
        }
        
        void p_dev_webglobs(WebHtml& h)
        {
            h.title("All WebGlobs");
            dev_table(h, web::glob_map());
        }

        void p_dev_webpages(WebHtml& h)
        {
            h.title("All WebPages");
            
            h << "<div class=\"explain\">"
              << "List of all registered (specific) web-pages.  Directories & GLOBs not included."
              << "</div>\n";
            
            dev_table(h, web::page_map(), true);
        }

        void p_dev_webvars(WebHtml& h)
        {
            h.title("All WebVariables");
            dev_table(h, web::variable_map());
        }
    
    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_server_pages()
        {
            reg_webpage<p_dev_echo>({hGet, hPost}, "/dev/echo");
            reg_webpage<p_dev_server>("/dev/server").local();
            reg_webpage<p_dev_webdirs>("/dev/web/directories");
            reg_webpage<p_dev_webexts>("/dev/web/extensions"); 
            reg_webpage<p_dev_webglobs>("/dev/web/globs");
            reg_webpage<p_dev_webpages>("/dev/web/pages");
            reg_webpage<p_dev_webvars>("/dev/web/variables");

        }
}
