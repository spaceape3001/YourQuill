////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math/vec/Vec2.hpp>
#include <math/shape/Size2.hpp>

namespace yq {
    template <typename T>
    struct Rect2 {
        Vec2<T>     position;
        Size2<T>    size;
        
        constexpr bool    operator==(const Rect2&) const noexcept = default;
    };
}
