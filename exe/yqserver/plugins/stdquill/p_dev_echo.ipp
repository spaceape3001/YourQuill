////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
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

    void    reg_dev_echo()
    {
        reg_webpage<p_dev_echo>({hGet, hPost}, "/dev/echo");
    }
}
