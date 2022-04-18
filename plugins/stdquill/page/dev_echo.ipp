////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    page_dev_echo(WebHtml& h)
    {
        auto& ctx   = h.context();

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
        {
            auto ta = h.table();
            for(auto& i : web_decode(ctx.decode_query()))
                h.kvrow(i.first) << i.second;
        }

        if(isPost){
            h.h2("Post Parameters");

            StringViewMultiMap   postParams = ctx.decode_post();
            std::string_view     url = postParams.first("url");
            if(!url.empty()){
                h << "<form action=\"" << url << "\" method=\"post\">\n";
                for(auto& i : postParams)
                    h << "<input type=\"hidden\" name=\"" << i.first << "\" id=\"" << i.first << "\" value=\"" << i.second << "\">\n";
                h << "<input type=\"submit\" value=\"Forward\">\n"
                  << "</form\n";
            }
            
            auto ta = h.table();
            for(auto& i : web_decode(postParams))
                h.kvrow(i.first) << i.second;
        }
        
        h.h2("Headers");
        {
            auto ta = h.table();
            for(auto& i : ctx.rx_headers)
                h.kvrow(i.first) << i.second;
        }
        
        if(!ctx.rx_body.empty()){
            h.h2("Body");
            h << ctx.rx_body;
        }
    }
    
    YQ_INVOKE( reg_webpage<page_dev_echo>({hGet, hPost}, "/dev/echo"); )
}
