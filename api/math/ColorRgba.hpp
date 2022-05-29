////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math/preamble.hpp>

namespace yq {
    template <typename T>
    struct ColorRgba {
        using component_type = T;
    
        T   red;
        T   green;
        T   blue;
        T   alpha;
        
        constexpr bool    operator==(const ColorRgba&) const noexcept = default;
    };
}
