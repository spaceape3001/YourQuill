////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq-toolbox/meta/InfoBinder.hpp>
#include <yq-toolbox/basic/Enum.hpp>

namespace yq {
    namespace mithril {

        /*! \enum AssertDeny
            \brief Assert/Deny something
            
            \var AssertDeny::Neither
            \brief No assertion/denial is being made, inherit it where possible
            
            \var AssertDeny::Deny
            \brief Explicity deny the thing
            
            \var AssertDeny::Assert
            \brief Explicity assert the thing
        */
        YQ_ENUM(AssertDeny, , Neither, Deny, Assert)
    }
}

YQ_TYPE_DECLARE(yq::mithril::AssertDeny)
