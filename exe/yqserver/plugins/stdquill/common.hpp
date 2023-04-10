////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/ByteArray.hpp>
#include <basic/DelayInit.hpp>
#include <basic/Guarded.hpp>
#include <basic/Logging.hpp>
#include <io/StreamOps.hpp>
#include <basic/TextUtils.hpp>

#include <mithril/preamble.hpp>

#include <mithril/web/PageTemplate.hpp>
#include <mithril/web/TypedBytes.hpp>
#include <mithril/web/Template.hpp>



//#include <db/html_common.hpp>


//#include <mithril/atom/Atom.hpp>
//#include <mithril/atom/Class.hpp>
//#include <mithril/atom/Field.hpp>
#include <mithril/bit/Copyright.hpp>
#include <mithril/enum/Submit.hpp>
//#include <mithril/leaf/Leaf.hpp>

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


void    update_css();

