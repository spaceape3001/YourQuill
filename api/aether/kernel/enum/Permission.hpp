////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <meta/InfoBinder.hpp>
#include <basic/Enum.hpp>
#include <basic/Flag.hpp>

namespace yq {

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

YQ_TYPE_DECLARE(yq::Permission)
YQ_TYPE_DECLARE(yq::Permissions)