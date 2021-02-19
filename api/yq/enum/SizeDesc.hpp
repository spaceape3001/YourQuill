////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "yq/util/Enum.hpp"
#include "yq/meta/MetaValue.hpp"

/*! 
    \enum SizeDesc
    \brief Generic size descriptor (for end users)
*/
YQ_ENUM(SizeDesc, ,
    Original,
    Large,
    Medium,
    Small
)

MV_DECLARE(SizeDesc)
