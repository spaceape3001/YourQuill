////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/app/DelayInit.hpp>
#include <yq/file/FileUtils.hpp>
#include <yq/http/HttpDataStream.hpp>
#include <yq/http/HttpRequest.hpp>
#include <yq/http/HttpResponse.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/web/WebPage.hpp>
#include <yq/web/WebHtml.hpp>
#include <yq/web/WebAdapters.hpp>

using namespace yq;

void    simpleTest(WebContext& ctx)
{
    WebHtml out(ctx, "TURTLE vs HARE");
    html::p(out) << "This is the contest between the turtle &amp; the hare.";
}


void    hello_world(WebContext& ctx)
{
    HttpDataStream  out(ctx.reply.content(ContentType::html));
    out << "<html><head><title>HELLO WORLD!</title></head>" 
        << "<body><h1>HELLO WORLD!</h1>\n<table>\n";
        
    out << "<tr><th align=\"left\">Method</th><td>" << ctx.request.method() << "</td></tr>\n";
    out << "<tr><th align=\"left\">URL</th><td>" << ctx.request.url() << "</td></tr>\n";
    out << "<tr><th align=\"left\">Client HTTP</th><td>" << ctx.request.version() << "</td></tr>\n";
    for(auto& hv : ctx.request.headers())
        out << "<tr><th align=\"left\">" << hv.key << "</th><td>" << hv.value << "</td><?tr>\n";
    out << "</table></body></html>\n";
}

void    test_directory(WebContext& ctx)
{
    HttpDataStream  out(ctx.reply.content(ContentType::html));
    
    out << "<html><head><title>TEST DIRECTORY!</title></head>" 
        << "<body><h1>TEST DIRECTORY!</h1>\n<table>\n";
        
    out << "<tr><th align=\"left\">PATH</th><td>" << ctx.truncated_path << "</td></tr>\n";
        
    out << "<tr><th align=\"left\">Method</th><td>" << ctx.request.method() << "</td></tr>\n";
    out << "<tr><th align=\"left\">URL</th><td>" << ctx.request.url() << "</td></tr>\n";
    out << "<tr><th align=\"left\">Client HTTP</th><td>" << ctx.request.version() << "</td></tr>\n";
    for(auto& hv : ctx.request.headers())
        out << "<tr><th align=\"left\">" << hv.key << "</th><td>" << hv.value << "</td><?tr>\n";
    out << "</table></body></html>\n";
}

YQ_INVOKE(
    reg_web("/readme",  std::filesystem::path(build_directory())/"README.md" );
    reg_webpage<simpleTest>("/hare");
    reg_webpage<hello_world>("/hello");
    reg_webpage<test_directory>("/test/**");
    reg_web("/yquill", std::filesystem::path(share_directory())/"www"/"img"/"yquill.svg");
);
