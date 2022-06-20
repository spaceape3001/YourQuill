////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/meta/InfoBinder.hpp>
#include <basic/Enum.hpp>
#include <basic/Flag.hpp>

namespace yq {
    YQ_ENUM(ShaderType, ,
        UNKNOWN,
        VERT,
        TESE,
        TESC,
        FRAG,
        GEOM,
        COMP
    )
    
    using ShaderTypeFlags   = Flag<ShaderType>;
}

YQ_TYPE_DECLARE(yq::ShaderType)
YQ_TYPE_DECLARE(yq::ShaderTypeFlags)
