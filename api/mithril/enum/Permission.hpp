////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/meta/InfoBinder.hpp>
#include <0/basic/Enum.hpp>
#include <0/basic/Flag.hpp>

namespace yq {

    namespace mithril {
        /*! \brief Access amount 
        */
        YQ_ENUM(Permission, ,
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
        )
        
        using Permissions  = Flag<Permission>;
    }
}

YQ_TYPE_DECLARE(yq::mithril::Permission)
YQ_TYPE_DECLARE(yq::mithril::Permissions)
