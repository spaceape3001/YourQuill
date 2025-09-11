////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include <mithril/root.hpp>
#include <mithril/html/HtmlLayout.hpp>
#include <mithril/util/Markdown.hpp>
#include <mithril/web/JsonAdapter.hpp>
#include <mithril/web/WebAdapters.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/web/WebHtml.hpp>
#include <mithril/web/WebImage.hpp>
#include <mithril/web/WebPage.hpp>
#include <mithril/web/WebRedirect.hpp>
#include <mithril/web/WebTemplate.hpp>
#include <mithril/web/Template.hpp>
#include <mithril/wksp/Workspace.hpp>

#include <yq/core/DelayInit.hpp>
#include <yq/file/DirUtils.hpp>
#include <yq/file/FileUtils.hpp>

using namespace std::string_view_literals;
using namespace yq;
using namespace yq::mithril;

#include "artdev.ipp"

namespace {
    path_vector_t       make_dir_set(std::string_view sname, std::string_view wname)
    {
        path_vector_t   ret;
        for(const RootDir* rt : wksp::root_dirs())
            ret.push_back(rt -> resolve(sname));
        for(auto& p : wksp::shared_all(wname))
            ret.push_back(p);
        return ret;
    }

    void reg_me()
    {
        reg_webpage("/art/**", make_dir_set(".art"sv, "www/art"sv));
    }
    
    YQ_INVOKE(reg_me();)
}

