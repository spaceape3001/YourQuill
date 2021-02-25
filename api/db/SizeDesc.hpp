////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "util/Enum.hpp"
#include "meta/MetaValue.hpp"

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
