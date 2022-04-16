////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "stdquill.hpp"

using namespace yq;

namespace {
    auto analyze(const WebPageMap& wm)
    {
        Map<std::string_view, EnumMap<HttpOp, const WebPage*>, IgCase> ret;
        for(HttpOp h : HttpOp::all_values()){
            for(auto& itr : wm[h])
                ret[itr.first][h] = itr.second;
        }
        return ret;
    }
    
    void    do_table(WebHtml& out, const WebPageMap& wm, bool links=false)
    {
        auto    t   = out.table();
        for(auto& i : analyze(wm)){
            out << "<TR><TH align=\"left\">" << i.first << "</TH>";
            for(HttpOp h : HttpOp::all_values()){
                out << "<TD>";
                const WebPage*pg = i.second[h];
                if(pg){
                    if(links && (h == hGet)){
                        out << "<a href=\"" << i.first << "\">" << h << "</a>";
                    } else 
                        out << h;
                    if(pg->local_only())
                        out << "<br><i>local</i>";
                }
                out << "</TD>";
            }
            out << "</TR>\n";
        }
    } 
    
    void    do_table(WebHtml& out, const WebVarMap& wm)
    {
        auto    t   = out.table();
        for(auto& i : wm){
            out << "<TR><TH align=\"left\">" << i.first << "</TH><TD>" << i.second->description() << "</TD></TR>\n";
        }
    }

    void    directories_table(WebHtml& out)
    {
        do_table(out, web::directory_map());
    }
    
    void    extensions_table(WebContext& ctx)
    {
        WebHtml  out(ctx, "All File Extensions");
        do_table(out, web::extension_map());
    }
    
    void    globs_table(WebContext& ctx)
    {
        WebHtml  out(ctx, "All GLOBs");
        do_table(out, web::glob_map());
    }
    
    void    pages_table(WebContext& ctx)
    {
        WebHtml  out(ctx, "All Pages");
        do_table(out, web::page_map(), true);
    }
    
    void    variables_table(WebContext& ctx)
    {
        WebHtml  out(ctx, "All Variables");
        do_table(out, web::variable_map());
    }
    
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
    
}

YQ_INVOKE(
    reg_webpage("/dev/**", wksp::shared_all("www/dev"sv));
    reg_webpage<page_dev_echo>({hGet, hPost}, "/dev/echo");
    reg_webpage<directories_table>("/dev/webdirs", "All Directories");
    reg_webpage<extensions_table>("/dev/webexts");
    reg_webpage<globs_table>("/dev/webglobs");
    reg_webpage<pages_table>("/dev/webpages");
    reg_webpage<variables_table>("/dev/webvars");
    //reg_web("markdown/*", wksp::shared_dir("www/markdown"));
)
