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

#include <aether/kernel/preamble.hpp>

#include <aether/http/web/PageTemplate.hpp>
#include <aether/http/web/TypedBytes.hpp>
#include <aether/http/web/Template.hpp>



//#include <db/html_common.hpp>


//#include <aether/yq/atom/Atom.hpp>
//#include <aether/yq/atom/Class.hpp>
//#include <aether/yq/atom/Field.hpp>
#include <aether/kernel/bit/Copyright.hpp>
#include <aether/kernel/enum/Submit.hpp>
//#include <aether/yq/leaf/Leaf.hpp>

#include <aether/http/web/WebContext.hpp>
#include <aether/http/web/WebHtml.hpp>
#include <aether/http/web/WebPage.hpp>
#include <aether/http/web/WebAdapters.hpp>
#include <aether/http/web/WebTemplate.hpp>


using namespace yq;

using SharedByteArray   = std::shared_ptr<ByteArray>;

extern Guarded<std::string>         gTextColor, gBkColor;
extern std::atomic<bool>            gHasBackground;
extern Guarded<SharedByteArray>     gCss;
extern std::filesystem::path        gSharedCssFile, gSharedPageFile;


void    update_css();

