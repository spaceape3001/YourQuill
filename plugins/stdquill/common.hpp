////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/type/ByteArray.hpp>
#include <basic/Guarded.hpp>
#include <yq/web/PageTemplate.hpp>
#include <yq/web/TypedBytes.hpp>
#include <yq/web/Template.hpp>
#include <string>

using namespace yq;

using SharedByteArray   = std::shared_ptr<ByteArray>;

extern Guarded<std::string>         gTextColor, gBkColor;
extern std::atomic<bool>            gHasBackground;
extern Guarded<SharedByteArray>     gCss;
extern std::filesystem::path        gSharedCssFile, gSharedPageFile;


void    update_css();

