////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <db/db.hpp>

#include <yq/app/DelayInit.hpp>
#include <yq/file/dir_utils.hpp>
#include <yq/file/file_utils.hpp>
#include <yq/log/Logging.hpp>
#include <yq/srv/NotifyAdapters.hpp>
#include <yq/srv/Stage2.hpp>
#include <yq/srv/Stage3.hpp>
#include <yq/srv/Stage4.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/stream/Text.hpp>

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



//  ====================================================================================================================
//      DO NOT RE-ORGANIZE as they are ORDER dependent.
//  ====================================================================================================================


//  DO NOT ORGANIZE theseblow, they will be listed in the desired order of execution


//#include "update/s3_image.ipp"
#include "update/s3_user.ipp"
#include "update/s3_tag.ipp"
//#include "update/s3_tag_leaf.ipp"

#include "update/image.ipp"
#include "update/tag.ipp"
#include "update/user.ipp"
