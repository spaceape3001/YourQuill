////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include <yq/mithril/root.hpp>
#include <yq/mithril/html/HtmlLayout.hpp>
#include <yq/mithril/util/Markdown.hpp>
#include <yq/mithril/web/JsonAdapter.hpp>
#include <yq/mithril/web/WebAdapters.hpp>
#include <yq/mithril/web/WebContext.hpp>
#include <yq/mithril/web/WebHtml.hpp>
#include <yq/mithril/web/WebImage.hpp>
#include <yq/mithril/web/WebPage.hpp>
#include <yq/mithril/web/WebRedirect.hpp>
#include <yq/mithril/web/WebTemplate.hpp>
#include <yq/mithril/web/Template.hpp>
#include <yq/mithril/wksp/Workspace.hpp>

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

