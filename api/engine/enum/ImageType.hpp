////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <basic/Enum.hpp>
#include <basic/meta/InfoBinder.hpp>

namespace yq {
    namespace engine {
        YQ_ENUM(ImageType, ,
            Is1D = 0,
            Is2D = 1,
            Is3D = 2
        )
    }
}
YQ_TYPE_DECLARE(yq::engine::ImageType)
