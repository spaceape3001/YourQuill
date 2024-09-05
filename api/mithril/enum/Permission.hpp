////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq-toolbox/meta/InfoBinder.hpp>
#include <yq-toolbox/basic/Enum.hpp>
#include <yq-toolbox/basic/Flag.hpp>

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
