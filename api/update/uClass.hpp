////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/trait/not_copyable.hpp>
#include <basic/trait/not_moveable.hpp>

#include <kernel/atom/Class.hpp>
#include <kernel/db/CacheFwd.hpp>
#include <kernel/enum/Change.hpp>

namespace yq {
    namespace update {

        //! Removes the class from the cache
        void    class_erase(Class);
        void    class_erase(Document);

        void    class_icon(Class);
        
        void    class_notify_icons(Fragment, Change);
        
        //!     First pass to make sure the class is created.
        //!     Advise this to be after category and tag creation
        void    class_stage3_pass1_create(Document);
    }
}
