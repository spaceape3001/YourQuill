////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/app/DelayInit.hpp>
#include <yq/file/NotifyAdapters.hpp>
#include <yq/http/HttpDataStream.hpp>
#include <yq/http/HttpRequest.hpp>
#include <yq/http/HttpResponse.hpp>
#include <yq/log/Logging.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/text/Utils.hpp>
#include <yq/type/Guarded.hpp>
#include <yq/web/WebPage.hpp>
#include <yq/web/WebAdapters.hpp>
#include <yq/web/WebTemplate.hpp>
#include <yq/wksp/Workspace.hpp>

using namespace yq;

namespace {
    void    var_abbr(Stream&str, WebContext&)
    {
        str << wksp::abbreviation();
    }

    void    var_author(Stream&str, WebContext&)
    {
        str << wksp::author();
    }

    void    var_home(Stream&str, WebContext&)
    {
        str << wksp::home();
    }

    void    var_host(Stream& str, WebContext&)
    {
        str << wksp::host();
    }

    void    var_name(Stream& str, WebContext&)
    {
        str << wksp::name();
    }

    void    var_port(Stream& str, WebContext&)
    {
        str << wksp::port();
    }
    
    void    var_time(Stream& str, WebContext& ctx)
    {
        str << ctx.time_text;
    }
}

YQ_INVOKE(

    unsigned int n = 0;
    for(auto& fs : wksp::shared_dirs())
        yInfo() << "share directory " << (++n) << ": "  << fs;

    if(!web::set_template(wksp::shared("std/page"sv)))
        yWarning() << "Failed to set web template!";

    reg_webpage("/img/**", wksp::shared_all("www/img"sv));
    reg_webpage("/help/*", wksp::shared_all("www/help"sv));
    reg_webpage("/js/*", wksp::shared_all("www/js"sv));

    
    reg_webvar<var_abbr>("abbr");
    reg_webvar<var_author>("author");
    reg_webvar<var_home>("home");
    reg_webvar<var_host>("host");
    reg_webvar<var_name>("name");
    reg_webvar<var_port>("port");
    reg_webvar<var_time>("time");

    //reg_web("img/**", wksp::shared_dir("www/img"));
    //reg_web("help/*", wksp::shared_dir("www/help"));
    //reg_web("js/*", wksp::shared_dir("www/js"));
    //reg_web("markdown/*", wksp::shared_dir("www/markdown"));
)

