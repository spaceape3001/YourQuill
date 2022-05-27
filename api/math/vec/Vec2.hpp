////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math/preamble.hpp>

namespace yq {
    template <typename T>
    struct Vec2 {
        using component_type = T;
        T   x, y;
        bool    operator==(const Vec2& rhs) const noexcept = default;
        static consteval Vec2 unit_x();
        static consteval Vec2 unit_y();
   };
}
