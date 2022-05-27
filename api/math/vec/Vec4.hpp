////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math/preamble.hpp>

namespace yq {

    template <typename T>
    struct Vec4 {
        using component_type = T;
        T  x, y, z, w;
        bool    operator==(const Vec4& rhs) const noexcept = default;
        static consteval Vec4 unit_x();
        static consteval Vec4 unit_y();
        static consteval Vec4 unit_z();
        static consteval Vec4 unit_w();
    };


}
