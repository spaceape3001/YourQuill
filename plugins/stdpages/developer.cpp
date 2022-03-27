////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/app/DelayInit.hpp>
#include <yq/http/HttpDataStream.hpp>
#include <yq/http/HttpRequest.hpp>
#include <yq/http/HttpResponse.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/web/WebPage.hpp>
#include <yq/web/WebAdapters.hpp>
#include <yq/wksp/Workspace.hpp>

using namespace yq;

namespace {
    auto analyze(const WebPageMap& wm)
    {
        Map<std::string_view, Flag<HttpOp>, IgCase> ret;
        for(HttpOp h : HttpOp::all_values()){
            for(auto& itr : wm[h])
                ret[itr.first] |= h;
        }
        return ret;
    }
    
    void    do_table(Stream& out, const WebPageMap& wm, bool links=false)
    {
        out << "<TABLE>\n";
        for(auto& i : analyze(wm)){
            out << "<TR><TH align=\"left\">" << i.first << "</TH><TD>\n";
            for(HttpOp h : HttpOp::all_values()){
                if(i.second.is_set(h)){
                    if(links && (h == hGet)){
                        out << "<a href=\"" << i.first << "\">" << h << "</a> ";
                    } else 
                        out << h << " ";
                }
            }
            out << "</TD></TR>\n";
        }
        out << "</TABLE>\n";
    } 
    
    void    do_table(Stream& out, const WebVarMap& wm)
    {
        out << "<TABLE>\n";
        for(auto& i : wm){
            out << "<TR><TH align=\"left\">" << i.first << "</TH><TD>" << i.second->description() << "</TD></TR>\n";
        }
        out << "</TABLE>\n";
    }

    void    directories_table(WebContext& ctx)
    {
        HttpDataStream  out(ctx.reply.content(ContentType::html));
        out << "<HTML><HEAD><TITLE>Registered Directories</TITLE></HEAD><BODY><H1>Available Web Directories</H1>\n";
        do_table(out, web::directory_map());
        out << "</BODY></HTML>\n";
    }
    
    void    extensions_table(WebContext& ctx)
    {
        HttpDataStream  out(ctx.reply.content(ContentType::html));
        out << "<HTML><HEAD><TITLE>Registered Extensions</TITLE></HEAD><BODY><H1>Available Web Extensions</H1>\n";
        do_table(out, web::extension_map());
        out << "</BODY></HTML>\n";
    }
    
    void    globs_table(WebContext& ctx)
    {
        HttpDataStream  out(ctx.reply.content(ContentType::html));
        out << "<HTML><HEAD><TITLE>Registered Recursive Directories</TITLE></HEAD><BODY><H1>Available Web Recursive Directories</H1>\n";
        do_table(out, web::glob_map());
        out << "</BODY></HTML>\n";
    }
    
    void    pages_table(WebContext& ctx)
    {
        HttpDataStream  out(ctx.reply.content(ContentType::html));
        out << "<HTML><HEAD><TITLE>Registered Pages</TITLE></HEAD><BODY><H1>Available Web Pages</H1>\n";
        do_table(out, web::page_map(), true);
        out << "</BODY></HTML>\n";
    }
    
    void    variables_table(WebContext& ctx)
    {
        HttpDataStream  out(ctx.reply.content(ContentType::html));
        out << "<HTML><HEAD><TITLE>Registered Variables</TITLE></HEAD><BODY><H1>Available Web Variables</H1>\n";
        do_table(out, web::variable_map());
        out << "</BODY></HTML>\n";
    }
    
}

YQ_INVOKE(
    reg_webpage<directories_table>("/dev/webdirs");
    reg_webpage<extensions_table>("/dev/webexts");
    reg_webpage<globs_table>("/dev/webglobs");
    reg_webpage<pages_table>("/dev/webpages");
    reg_webpage<variables_table>("/dev/webvars");
    //reg_web("img/**", wksp::shared_dir("www/img"));
    //reg_web("help/*", wksp::shared_dir("www/help"));
    //reg_web("js/*", wksp::shared_dir("www/js"));
    //reg_web("markdown/*", wksp::shared_dir("www/markdown"));
)
