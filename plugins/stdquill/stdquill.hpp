////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/app/DelayInit.hpp>
#include <yq/bit/Copyright.hpp>
#include <yq/file/cdb.hpp>
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
#include <yq/wksp/Root.hpp>
#include <yq/wksp/Workspace.hpp>

extern yq::Guarded<std::string>     gTextColor, gBkColor;

