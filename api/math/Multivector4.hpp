////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math/preamble.hpp>
#include <math/Vector4.hpp>
#include <math/Bivector4.hpp>
#include <math/Trivector4.hpp>
#include <math/Quadvector4.hpp>

namespace yq {
    template <typename T>
    struct Multivector4 {
        unity_t<T>      b0;
        Vector4<T>      b1;
        Bivector4<T>    b2;
        Trivector4<T>   b3;
        Quadvector4<T>  b4;
        
        bool operator==(const Multivector4&) const = default;
    };

    YQ_IEEE754_1(Multivector4)
    YQ_NAN_1(Multivector4, Multivector4<T>{nan_v<unity_t<T>>, nan_v<Vector4<T>>, nan_v<Bivector4<T>>, nan_v<Trivector4<T>>, nan_v<Quadvector4<T>>})
    YQ_ZERO_1(Multivector4, Multivector4<T>{zero_v<unity_t<T>>, zero_v<Vector4<T>>, zero_v<Bivector4<T>>, zero_v<Trivector4<T>>, zero_v<Quadvector4<T>>})
    
    YQ_IS_NAN_1(Multivector4, is_nan(v.b0) || is_nan(v.b1) || is_nan(v.b2) || is_nan(v.b3) || is_nan(v.b4) )
    YQ_IS_FINITE_1(Multivector4, is_finite(v.b0) && is_finite(v.b1) && is_finite(v.b2) && is_finite(v.b3) && is_finite(v.b4))
}

YQ_TYPE_DECLARE(Multivector4D)
YQ_TYPE_DECLARE(Multivector4F)

