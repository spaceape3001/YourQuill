#include <yq/app/DelayInit.hpp>
#include <yq/http/HttpDataStream.hpp>
#include <yq/http/HttpRequest.hpp>
#include <yq/http/HttpResponse.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/web/Web.hpp>
#include <yq/wksp/Workspace.hpp>

using namespace yq;

YQ_INVOKE(
    //reg_web("img/**", wksp::shared_dir("www/img"));
    //reg_web("help/*", wksp::shared_dir("www/help"));
    //reg_web("js/*", wksp::shared_dir("www/js"));
    //reg_web("markdown/*", wksp::shared_dir("www/markdown"));
)
