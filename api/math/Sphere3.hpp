////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math/preamble.hpp>
#include <math/Vector3.hpp>

namespace yq {
    template <typename T>
    struct Sphere3 {
        using component_t   = T;

        Vector3<T>  point;
        T           radius;
        
        constexpr bool operator==(const Sphere3&) const noexcept = default;
    };

//  --------------------------------------------------------
//  COMPOSITION
    
    template <typename T>
    Sphere3<T>  sphere(const Vector3<T>& point, T radius)
    {
        return {point, radius};
    }

    YQ_NAN_1(Sphere3, { nan_v<Vector3<T>>, nan_v<T> })
    YQ_ZERO_1(Sphere3, { zero_v<Vector3<T>>, zero_v<T> })

//  --------------------------------------------------------
//  BASIC FUNCTIONS
    
    YQ_IS_FINITE_1(Sphere3, is_finite(v.point) && is_finite(v.radius))
    YQ_IS_NAN_1(Sphere3, is_nan(v.point) || is_nan(v.radius))

//  --------------------------------------------------------
//  ADVANCED FUNCTIONS

    template <typename T>
    T           diameter(const Sphere3<T>&a)
    {
        return a.radius + a.radius;
    }
}

YQ_TYPE_DECLARE(yq::Sphere3D)
YQ_TYPE_DECLARE(yq::Sphere3F)
YQ_TYPE_DECLARE(yq::Sphere3I)
YQ_TYPE_DECLARE(yq::Sphere3U)

