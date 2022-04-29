////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "common.hpp"
#include <db/db.hpp>

#include <yq/app/DelayInit.hpp>
#include <yq/file/dir_utils.hpp>
#include <yq/file/file_utils.hpp>
#include <yq/log/Logging.hpp>
#include <yq/srv/NotifyAdapters.hpp>
#include <yq/srv/Stage2.hpp>
#include <yq/srv/Stage3.hpp>
#include <yq/srv/Stage4.hpp>
#include <yq/stream/Bytes.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/stream/Text.hpp>
#include <yq/text/text_utils.hpp>


using namespace yq;
using namespace yq::cdb;

namespace {
    int     s2_counter      = 0;
    int     s3_counter      = 0;
    int     s4_counter      = 0;
    int     n_counter  = 0;
    
    //  And yes, the above will go negative
    
    template <void (*FN)()>
    void    u_stage2(const std::source_location&sl = std::source_location::current())
    {
        on_stage2<FN>(--s2_counter, sl);
    }

    template <void (*FN)()>
    void    u_stage3(std::string_view name, const std::source_location&sl = std::source_location::current())
    {
        on_stage3<FN>(--s3_counter, name, sl);
    }

    template <void (*FN)()>
    void    u_stage3(Folder f, std::string_view name, const std::source_location&sl = std::source_location::current())
    {
        on_stage3<FN>(--s3_counter, f, name, sl);
    }
    
    template <void (*FN)(Document)>
    void    u_stage3(std::string_view name, const std::source_location&sl = std::source_location::current())
    {
        on_stage3<FN>(--s3_counter, name, sl);
    }

    template <void (*FN)(Document)>
    void    u_stage3(Folder f, std::string_view name, const std::source_location&sl = std::source_location::current())
    {
        on_stage3<FN>(--s3_counter, f, name, sl);
    }

    template <void (*FN)()>
    void    u_stage4(const std::source_location&sl = std::source_location::current())
    {
        on_stage4<FN>(--s4_counter, sl);
    }

    template <void (*FN)()> 
    Notifier::Writer    u_change(const std::filesystem::path&fp, const std::source_location& sl = std::source_location::current())
    {
        return on_change<FN>(--n_counter, fp, sl);
    }

    template <void (*FN)()> 
    Notifier::Writer    u_change(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return on_change<FN>(--n_counter, f, ext, sl);
    }

    template <void (*FN)(Fragment)> 
    Notifier::Writer    u_change(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return on_change<FN>(--n_counter, ext, sl);
    }

    template <void (*FN)(Fragment)> 
    Notifier::Writer    u_change(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return on_change<FN>(--n_counter, f, ext, sl);
    }

    template <void (*FN)(Fragment)> 
    Notifier::Writer    u_change(ChangeFlags ch, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return on_chnage<FN>(--n_counter, ch, ext, sl);
    }

    template <void (*FN)(Fragment)> 
    Notifier::Writer    u_change(ChangeFlags ch, Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return on_change<FN>(--n_counter, ch, f, ext, sl);
    }

    template <void (*FN)(Fragment, Change)> 
    Notifier::Writer    u_change(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return on_change<FN>(--n_counter, ext, sl);
    }

    template <void (*FN)(Fragment, Change)> 
    Notifier::Writer    u_change(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return on_change<FN>(--n_counter, f, ext, sl);
    }

    template <void (*FN)(Fragment, Change)> 
    Notifier::Writer    u_change(ChangeFlags ch, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return on_change<FN>(--n_counter, ch, ext, sl);
    }

    template <void (*FN)(Fragment, Change)> 
    Notifier::Writer    u_change(ChangeFlags ch, Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return on_change<FN>(--n_counter, ch, f, ext, sl);
    }
}

Guarded<std::string>            gTextColor, gBkColor;
Guarded<Ref<PageTemplate>>      gIndex;
Guarded<Ref<WebTemplate>>       gFooter, gSummary;
Guarded<Ref<TypedBytes>>        gBackground;
std::atomic<bool>               gHasBackground{false};
Guarded<SharedByteArray>        gCss;
std::vector<std::string>        gBackgroundFiles;
std::filesystem::path           gSharedCssFile, gSharedFooterFile, gSharedIndexFile, gSharedPageFile, gSharedSummaryFile;

#include "fcn/load_background.ipp"
#include "fcn/update_css.ipp"
#include "fcn/update_footer.ipp"
#include "fcn/update_index.ipp"
#include "fcn/update_page.ipp"
#include "fcn/update_summary.ipp"

//  ====================================================================================================================
//      DO NOT RE-ORGANIZE the following as they are ORDER dependent.
//  ====================================================================================================================


//  DO NOT ORGANIZE theseblow, they will be listed in the desired order of execution

#include "update/s2_image.ipp"

#include "update/s3_image.ipp"
#include "update/s3_user.ipp"
#include "update/s3_tag.ipp"
#include "update/s3_category.ipp"
#include "update/s3_class.ipp"
#include "update/s3_field.ipp"
#include "update/s3_class_field.ipp"
#include "update/s3_leaf.ipp"

//#include "update/s3_tag_leaf.ipp"

#include "update/s4_background.ipp"
#include "update/s4_css.ipp"
#include "update/s4_footer.ipp"
#include "update/s4_index.ipp"
#include "update/s4_page.ipp"
#include "update/s4_summary.ipp"

#include "update/u_image.ipp"
#include "update/u_user.ipp"
#include "update/u_tag.ipp"
#include "update/u_category.ipp"

#include "update/u_class.ipp"
#include "update/u_field.ipp"

#include "update/u_background.ipp"
#include "update/u_css.ipp"
#include "update/u_index.ipp"
#include "update/u_footer.ipp"
#include "update/u_page.ipp"
#include "update/u_summary.ipp"


