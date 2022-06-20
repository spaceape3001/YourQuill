////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TriangleData.hpp"
#include <math/preamble.hpp>
#include <math/Vector3.hpp>

namespace yq {

//  --------------------------------------------------------
//  COMPOSITION

    template <typename T>
    Triangle3<T>    triangle(const Vector3<T>& a, const Vector3<T>& b, const Vector3<T>& c)
    {
        return { a, b, c };
    }

    YQ_NAN_1(Triangle3, { nan_v<Vector3<T>>, nan_v<Vector3<T>>, nan_v<Vector3<T>> })
    YQ_ZERO_1(Triangle3, { zero_v<Vector3<T>>, zero_v<Vector3<T>>, zero_v<Vector3<T>> })

//  --------------------------------------------------------
//  BASIC FUNCTIONS

    YQ_IS_FINITE_1(Triangle3, is_finite(v.a) && is_finite(v.b) && is_finite(v.c))
    YQ_IS_NAN_1(Triangle3, is_nan(v.a) || is_nan(v.b) || is_nan(v.c) )

//  --------------------------------------------------------
//  ADVANCED FUNCTIONS

}

YQ_TYPE_DECLARE(yq::Triangle3D)
YQ_TYPE_DECLARE(yq::Triangle3F)
YQ_TYPE_DECLARE(yq::Triangle3I)
YQ_TYPE_DECLARE(yq::Triangle3U)

