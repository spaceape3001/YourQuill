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

#include <http/PageTemplate.hpp>
#include <http/Post.hpp>
#include <http/TypedBytes.hpp>
#include <http/Template.hpp>



//#include <db/html_common.hpp>


//#include <yq/atom/Atom.hpp>
//#include <yq/atom/Class.hpp>
//#include <yq/atom/Field.hpp>
#include <kernel/Copyright.hpp>
#include <kernel/Submit.hpp>
//#include <yq/leaf/Leaf.hpp>

#include <http/WebContext.hpp>
#include <http/WebHtml.hpp>
#include <http/WebPage.hpp>
#include <http/WebAdapters.hpp>
#include <http/WebTemplate.hpp>


using namespace yq;

using SharedByteArray   = std::shared_ptr<ByteArray>;

extern Guarded<std::string>         gTextColor, gBkColor;
extern std::atomic<bool>            gHasBackground;
extern Guarded<SharedByteArray>     gCss;
extern std::filesystem::path        gSharedCssFile, gSharedPageFile;


void    update_css();

