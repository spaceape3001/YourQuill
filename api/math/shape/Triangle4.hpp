////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math/preamble.hpp>
#include <math/vec/Vector4.hpp>

namespace yq {
    template <typename T>
    struct Triangle4 {
        using component_t   = T;

        Vector4<T>     a, b, c;

        constexpr bool operator==(const Triangle4&) const noexcept = default;
    };

//  --------------------------------------------------------
//  COMPOSITION

    template <typename T>
    Triangle4<T>    triangle(const Vector4<T>& a, const Vector4<T>& b, const Vector4<T>& c)
    {
        return { a, b, c };
    }

    YQ_NAN_1(Triangle4, { nan_v<Vector3<T>>, nan_v<Vector3<T>>, nan_v<Vector3<T>> })
    YQ_ZERO_1(Triangle4, { zero_v<Vector3<T>>, zero_v<Vector3<T>>, zero_v<Vector3<T>> })

//  --------------------------------------------------------
//  BASIC FUNCTIONS

    YQ_IS_FINITE_1(Triangle4, is_finite(v.a) && is_finite(v.b) && is_finite(v.c))
    YQ_IS_NAN_1(Triangle4, is_nan(v.a) || is_nan(v.b) || is_nan(v.c) )

//  --------------------------------------------------------
//  ADVANCED FUNCTIONS

}

YQ_TYPE_DECLARE(yq::Triangle4D)
YQ_TYPE_DECLARE(yq::Triangle4F)
YQ_TYPE_DECLARE(yq::Triangle4I)
YQ_TYPE_DECLARE(yq::Triangle4U)
