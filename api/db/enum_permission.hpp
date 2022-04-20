////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/Binder.hpp>
#include <yq/type/Enum.hpp>
#include <yq/type/Flag.hpp>

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
        Reader
    )
    
    using Permissions  = Flag<Permission>;
}

YQ_TYPE_DECLARE(yq::Permission)
YQ_TYPE_DECLARE(yq::Permissions)
