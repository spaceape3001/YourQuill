////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/app/DelayInit.hpp>
#include <yq/http/HttpRequest.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/text/Utils.hpp>
#include <yq/web/WebHtml.hpp>
#include <yq/web/WebPage.hpp>
#include <yq/web/WebAdapters.hpp>
#include <yq/wksp/Root.hpp>
#include <yq/wksp/Workspace.hpp>

using namespace yq;

namespace {
    void    dev_table(WebHtml& h, const root_vector_t& roots)
    {
        auto t = html::table(h);
        h << "<tr><th><ID></th><th>Key</th><th>Name</th><th>Path</th></tr>\n";
        for(const Root* r : roots){
            if(!r)
                continue;
            h << "<tr><td><a href=\"/dev/root?id=" << r->id << "\">" << r->id << "</a></td><td>" << r->key 
                << "</td><td>" << r->name << "</td><td>" << r->path << "</td></tr>\n";
        }
    }

    void    wksp_roots(WebHtml&h)
    {
        h.title("Workspace Roots");
        dev_table(h, wksp::roots());
    }
}

YQ_INVOKE(
    reg_webpage<wksp_roots>("/dev/wksp/roots");
)
