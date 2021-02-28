////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <meta/MetaValue.hpp>
#include <util/Enum.hpp>

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
