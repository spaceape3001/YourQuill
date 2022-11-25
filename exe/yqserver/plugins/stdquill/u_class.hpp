////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/trait/not_copyable.hpp>
#include <basic/trait/not_moveable.hpp>

#include <kernel/agw/Class.hpp>
#include <kernel/db/CacheFwd.hpp>
#include <kernel/enum/Change.hpp>

namespace yq {
    //! Removes the class from the cache
    void    u_class_erase(Class);
    void    u_class_erase(Document);

    void    u_class_icon(Class);
    
    void    u_class_notify(Fragment, Change);
    
    void    u_class_notify_icons(Fragment, Change);
    
    //!     First pass to make sure the class is created.
    //!     Advise this to be after category and tag creation
    void    u_class_stage3_pass1_create(Document);
    
    //!     Registers the "base"/use classes
    void    u_class_stage3_pass2_bind(Document);
    
    //!     Extends the base classes all the way
    void    u_class_stage3_pass3_extend(Document);
    
    //!     Deduces targets/sources/etc from the class (after fields)
    void    u_class_stage3_pass4_deduce(Document);
    
    void    u_class_update(Class);
}
