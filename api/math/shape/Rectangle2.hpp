////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math/vec/Vector2.hpp>
#include <math/shape/Size2.hpp>

namespace yq {
    template <typename T>
    struct Rectangle2 {
        Vector2<T>     position;
        Size2<T>    size;
        
        constexpr bool    operator==(const Rectangle2&) const noexcept = default;
    };
}
