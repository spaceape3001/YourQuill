////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math/preamble.hpp>
#include <math/Vector2.hpp>
#include <math/Bivector2.hpp>

namespace yq {
    template <typename T>
    struct Multivector2 {
        unity_t<T>      b0;
        Vector2<T>      b1;
        Bivector2<T>    b2;
        
        bool operator==(const Multivector2&) const = default;
    };

    YQ_IEEE754_1(Multivector2)
    YQ_NAN_1(Multivector2, Multivector2<T>{nan_v<unity_t<T>>, nan_v<Vector2<T>>, nan_v<Bivector2<T>>})
    YQ_ZERO_1(Multivector2, Multivector2<T>{zero_v<unity_t<T>>, zero_v<Vector2<T>>, zero_v<Bivector2<T>>})
    
    YQ_IS_NAN_1(Multivector2, is_nan(v.b0) || is_nan(v.b1) || is_nan(v.b2) )
    YQ_IS_FINITE_1(Multivector2, is_finite(v.b0) && is_finite(v.b1) && is_finite(v.b2))
}

YQ_TYPE_DECLARE(Multivector2D)
YQ_TYPE_DECLARE(Multivector2F)
