////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math/preamble.hpp>

namespace yq {
    template <typename T>
    struct Vec3 {
        using component_type = T;
        T  x, y, z;
        bool    operator==(const Vec3& rhs) const noexcept = default;
        static consteval Vec3 unit_x();
        static consteval Vec3 unit_y();
        static consteval Vec3 unit_z();
    };
}
