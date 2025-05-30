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


#include <mithril/preamble.hpp>

#include <mithril/web/PageTemplate.hpp>
#include <mithril/web/TypedBytes.hpp>
#include <mithril/web/Template.hpp>


#include <mithril/bit/Copyright.hpp>
#include <mithril/enum/Submit.hpp>

#include <mithril/web/WebContext.hpp>
#include <mithril/web/WebHtml.hpp>
#include <mithril/web/WebPage.hpp>
#include <mithril/web/WebAdapters.hpp>
#include <mithril/web/WebTemplate.hpp>


using namespace yq;
using namespace yq::mithril;

using SharedByteArray   = std::shared_ptr<ByteArray>;

extern Guarded<std::string>         gTextColor, gBkColor;
extern std::atomic<bool>            gHasBackground;
extern Guarded<SharedByteArray>     gCss;
extern std::filesystem::path        gSharedCssFile, gSharedPageFile;
extern std::atomic<uint64_t>        gClassGraph;


void    update_css();

