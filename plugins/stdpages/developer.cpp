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
#include <yq/text/Utils.hpp>
#include <yq/web/WebHtml.hpp>
#include <yq/web/WebPage.hpp>
#include <yq/web/WebAdapters.hpp>
#include <yq/wksp/Workspace.hpp>

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
        auto    t   = html::table(out);
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
        auto    t   = html::table(out);
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
    
}

YQ_INVOKE(
    reg_webpage<directories_table>("/dev/webdirs", "All Directories");
    reg_webpage<extensions_table>("/dev/webexts");
    reg_webpage<globs_table>("/dev/webglobs");
    reg_webpage<pages_table>("/dev/webpages");
    reg_webpage<variables_table>("/dev/webvars");
    //reg_web("markdown/*", wksp::shared_dir("www/markdown"));
)
