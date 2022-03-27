////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/app/DelayInit.hpp>
#include <yq/http/HttpDataStream.hpp>
#include <yq/http/HttpRequest.hpp>
#include <yq/http/HttpResponse.hpp>
#include <yq/log/Logging.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/text/Utils.hpp>
#include <yq/web/WebPage.hpp>
#include <yq/web/WebAdapters.hpp>
#include <yq/wksp/Workspace.hpp>

using namespace yq;

YQ_INVOKE(

    unsigned int n = 0;
    for(auto& fs : wksp::shared_dirs())
        yInfo() << "share directory " << (++n) << ": "  << fs;

    if(!web::set_template(wksp::shared("std/page"sv)))
        yWarning() << "Failed to set web template!";

    //reg_web("img/**", wksp::shared_dir("www/img"));
    //reg_web("help/*", wksp::shared_dir("www/help"));
    //reg_web("js/*", wksp::shared_dir("www/js"));
    //reg_web("markdown/*", wksp::shared_dir("www/markdown"));
)

