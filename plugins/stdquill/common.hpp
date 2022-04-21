////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "imaging.hpp"

#include <yq/type/ByteArray.hpp>
#include <yq/type/Guarded.hpp>
#include <yq/web/PageTemplate.hpp>
#include <yq/web/TypedBytes.hpp>
#include <yq/web/WebTemplate.hpp>
#include <string>


using namespace yq;

using SharedByteArray   = std::shared_ptr<ByteArray>;

extern Guarded<std::string>         gTextColor, gBkColor;
extern Guarded<Ref<PageTemplate>>   gIndex;
extern Guarded<Ref<WebTemplate>>    gFooter, gSummary;
extern Guarded<Ref<TypedBytes>>     gBackground;
extern std::atomic<bool>            gHasBackground;
extern Guarded<SharedByteArray>     gCss;
extern std::vector<std::string>     gBackgroundFiles;
extern std::filesystem::path        gSharedCssFile, gSharedFooterFile, gSharedIndexFile, gSharedPageFile, gSharedSummaryFile;


static constexpr const char*    kPage       = ".page";
static constexpr const char*    kBackground = ".background";

static const std::string_view       kStdCSS         = "std/css";
static const std::string_view       kStdPage        = "std/page";
static const std::string_view       kStdIndex       = "std/index";
static const std::string_view       kStdFooter      = "std/footer";
static const std::string_view       kStdSummary     = "std/summary";

static const std::initializer_list<std::string_view>    kIndexFiles = { ".index", ".index.html", ".index.htm" };


//  Returns TRUE if something has changed
bool    load_background(unsigned int opts=0);

//  Updates the CSS scheme
void    update_css(unsigned int opts=0);
void    update_footer(unsigned int opts=0);
void    update_summary(unsigned int opts=0);
void    update_page(unsigned int opts=0);
void    update_index(unsigned int opts=0);
