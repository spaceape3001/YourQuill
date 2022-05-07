////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/type/ByteArray.hpp>
#include <yq/type/Guarded.hpp>
#include <yq/web/PageTemplate.hpp>
#include <yq/web/TypedBytes.hpp>
#include <yq/web/Template.hpp>
#include <string>

using namespace yq;

using SharedByteArray   = std::shared_ptr<ByteArray>;

extern Guarded<std::string>         gTextColor, gBkColor;
extern Guarded<Ref<TypedBytes>>     gBackground;
extern std::atomic<bool>            gHasBackground;
extern Guarded<SharedByteArray>     gCss;
extern std::vector<std::string>     gBackgroundFiles;
extern std::filesystem::path        gSharedCssFile, gSharedPageFile;


