////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "yq/meta/MetaValue.hpp"
#include "yq/util/Enum.hpp"

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
MV_DECLARE(AssertDeny)

