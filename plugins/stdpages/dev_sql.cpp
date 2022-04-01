////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/app/DelayInit.hpp>
#include <yq/http/HttpDataStream.hpp>
#include <yq/http/HttpRequest.hpp>
#include <yq/http/HttpResponse.hpp>
#include <yq/sql/SqlLite.hpp>
#include <yq/sql/SqlQuery.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/web/WebHtml.hpp>
#include <yq/web/WebPage.hpp>
#include <yq/web/WebAdapters.hpp>
#include <yq/wksp/Workspace.hpp>

using namespace yq;

namespace {
    void page_tables(WebHtml& out)
    {
        auto list = html::numbers(out);
        for(auto& s : wksp::db().tables())
            html::li(out) << s;
    }
}

YQ_INVOKE(
    reg_webpage<page_tables>("/dev/sqltables", "SQL Tables").local();
)
