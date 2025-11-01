////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/container/ByteArray.hpp>
#include <yq/core/DelayInit.hpp>
#include <yq/core/Guarded.hpp>
#include <yq/core/Logging.hpp>
#include <yq/core/StreamOps.hpp>


#include <yq/mithril/preamble.hpp>

#include <yq/mithril/web/PageTemplate.hpp>
#include <yq/mithril/web/TypedBytes.hpp>
#include <yq/mithril/web/Template.hpp>


#include <yq/mithril/bit/Copyright.hpp>
#include <yq/mithril/enum/Submit.hpp>

#include <yq/mithril/web/WebContext.hpp>
#include <yq/mithril/web/WebHtml.hpp>
#include <yq/mithril/web/WebPage.hpp>
#include <yq/mithril/web/WebAdapters.hpp>
#include <yq/mithril/web/WebTemplate.hpp>


using namespace yq;
using namespace yq::mithril;

using SharedByteArray   = std::shared_ptr<ByteArray>;

extern Guarded<std::string>         gTextColor, gBkColor;
extern std::atomic<bool>            gHasBackground;
extern Guarded<SharedByteArray>     gCss;
extern std::filesystem::path        gSharedCssFile, gSharedPageFile;
extern std::atomic<uint64_t>        gClassGraph;


void    update_css();

