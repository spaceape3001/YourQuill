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
#include <basic/StreamOps.hpp>
#include <basic/TextUtils.hpp>

#include <kernel/preamble.hpp>

#include <http/web/PageTemplate.hpp>
#include <http/web/TypedBytes.hpp>
#include <http/web/Template.hpp>



//#include <db/html_common.hpp>


//#include <yq/atom/Atom.hpp>
//#include <yq/atom/Class.hpp>
//#include <yq/atom/Field.hpp>
#include <kernel/bit/Copyright.hpp>
#include <kernel/enum/Submit.hpp>
//#include <yq/leaf/Leaf.hpp>

#include <http/web/WebContext.hpp>
#include <http/web/WebHtml.hpp>
#include <http/web/WebPage.hpp>
#include <http/web/WebAdapters.hpp>
#include <http/web/WebTemplate.hpp>


using namespace yq;

using SharedByteArray   = std::shared_ptr<ByteArray>;

extern Guarded<std::string>         gTextColor, gBkColor;
extern std::atomic<bool>            gHasBackground;
extern Guarded<SharedByteArray>     gCss;
extern std::filesystem::path        gSharedCssFile, gSharedPageFile;


void    update_css();

