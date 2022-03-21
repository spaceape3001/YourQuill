#include <io/http/HttpResponse.hpp>
#include <io/http/HttpDataStream.hpp>
#include <io/web/WebPage.hpp>

#include <util/app/DelayInit.hpp>
#include <util/stream/Ops.hpp>

using namespace yq;

void    simpleTest(const HttpRequest&, HttpResponse& rs)
{
    HttpDataStream  out(rs.content(ContentType::html));
    
    out <<
        "<HTML><HEAD><TITLE>SIMPLE TEST</TITLE></HEAD><BODY><H1>TURTLE vs HARE</H1><p>This is the contest between the turtle &amp; the hare.</p></BODY></HTML>";
}

YQ_INVOKE(
    reg_webpage("/hare", simpleTest);
);
