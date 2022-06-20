////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SimpleSpace.hpp"
#include <math/tensor_math.hpp>

namespace yq {
    Tensor44D       SimpleSpace::local2parent() const
    {
        Tensor33D   T   = tensor(orientation) * diagonal(scale);
        return Tensor44D(
            T.xx, T.xy, T.xz, position.x,
            T.yx, T.yy, T.yz, position.y,
            T.zx, T.zy, T.zz, position.z,
            0., 0., 0., 1.
        );
    }
}

