////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/app/DelayInit.hpp>
#include <yq/file/cdb.hpp>
#include <yq/file/FileUtils.hpp>
#include <yq/srv/NotifyAdapters.hpp>
#include <yq/log/Logging.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/web/HttpDataStream.hpp>
#include <yq/web/WebAdapters.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>
#include <yq/web/WebPage.hpp>

using namespace yq;

void    simpleTest(WebHtml& out)
{
    out.title("TURTLE vs HARE");
    html::paragraph(out) << "This is the contest between the turtle &amp; the hare.";
}


void    hello_world(WebHtml& out)
{
    out.title("HELLO WORLD!");
    
    auto _t     = html::table(out);
    
    html::kvrow(out, "Method") << out.context().method.key() << "\n";
    html::kvrow(out, "URL") << out.context().url << "\n";
    html::kvrow(out, "Client HTTP") << out.context().version << "</td></tr>\n";
    for(auto& hv : out.context().rx_headers)
        html::kvrow(out, hv.key) << hv.value;
}

void    test_directory(WebContext& ctx)
{
    HttpDataStream  out(ctx.tx_content);
    ctx.tx_content_type = ContentType::html;
    
    out << "<html><head><title>TEST DIRECTORY!</title></head>" 
        << "<body><h1>TEST DIRECTORY!</h1>\n<table>\n";
        
    out << "<tr><th align=\"left\">PATH</th><td>" << ctx.truncated_path << "</td></tr>\n";
        
    out << "<tr><th align=\"left\">Method</th><td>" << ctx.method << "</td></tr>\n";
    out << "<tr><th align=\"left\">URL</th><td>" << ctx.url << "</td></tr>\n";
    out << "<tr><th align=\"left\">Client HTTP</th><td>" << ctx.version << "</td></tr>\n";
    for(auto& hv : ctx.rx_headers)
        out << "<tr><th align=\"left\">" << hv.key << "</th><td>" << hv.value << "</td><?tr>\n";
    out << "</table></body></html>\n";
}

void    file_hare(Fragment f, Change c)
{
    yInfo() << "Hare today ... (" << c.key() << ") " << cdb::path(f);
}

void    file_turtle(Fragment f, Change c)
{
    yInfo() << "Turtle tomorrow  ... (" << c.key() << ") " << cdb::path(f);
}

YQ_INVOKE(
    reg_webpage("/readme",  std::filesystem::path(build_directory())/"README.md" );
    reg_webpage<simpleTest>("/hare");
    reg_webpage<hello_world>("/hello");
    reg_webpage<test_directory>("/test/**");
    reg_webpage("/yquill", std::filesystem::path(share_directory())/"www"/"img"/"yquill.svg");
    
    on_change<file_hare>("*.hare");
    on_change<file_turtle>("*.turtle");
);
