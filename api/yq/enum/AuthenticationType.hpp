////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/Binder.hpp>
#include <yq/type/Enum.hpp>

namespace yq {

    /*! \brief Type of authentication

    */
    YQ_ENUM(AuthenticationType, , 
    
        // NOTE!  Order these in terms of least-preference-first
    
        None,       //!< Generally NOT recommended
        Plain       //!< Not terribly secure (plain password)
    )
}

YQ_TYPE_DECLARE(yq::AuthenticationType)
