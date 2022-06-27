////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math/preamble.hpp>
#include <math/Vector3.hpp>
#include <math/Bivector3.hpp>
#include <math/Trivector3.hpp>

namespace yq {
    template <typename T>
    struct Multivector3 {
        unity_t<T>      b0;
        Vector3<T>      b1;
        Bivector3<T>    b2;
        Trivector3<T>   b3;
        
        bool operator==(const Multivector3&) const = default;
    };

    YQ_IEEE754_1(Multivector3)
    YQ_NAN_1(Multivector3, Multivector3<T>{nan_v<unity_t<T>>, nan_v<Vector3<T>>, nan_v<Bivector3<T>>, nan_v<Trivector3<T>>})
    YQ_ZERO_1(Multivector3, Multivector3<T>{zero_v<unity_t<T>>, zero_v<Vector3<T>>, zero_v<Bivector3<T>>, zero_v<Trivector3<T>>})
    
    YQ_IS_NAN_1(Multivector3, is_nan(v.b0) || is_nan(v.b1) || is_nan(v.b2) || is_nan(v.b3) )
    YQ_IS_FINITE_1(Multivector3, is_finite(v.b0) && is_finite(v.b1) && is_finite(v.b2) && is_finite(v.b3))
}

YQ_TYPE_DECLARE(Multivector3D)
YQ_TYPE_DECLARE(Multivector3F)
