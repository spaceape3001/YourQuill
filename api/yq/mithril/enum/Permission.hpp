////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/core/Flags.hpp>
#include <yq/meta/MetaBinder.hpp>

namespace yq::mithril {
    /*! \brief Access amount 
    */
    enum class Permission : uint8_t {
        //! Website owner
        Owner,
        //! Administrative rights
        Admin,
        //! Author rights
        Writer,
        //! Reading rights
        Reader,
        //! Guest role
        Guest
    };
    
    using Permissions  = Flags<Permission, uint8_t>;
}

YQ_TYPE_DECLARE(yq::mithril::Permission)
YQ_TYPE_DECLARE(yq::mithril::Permissions)
