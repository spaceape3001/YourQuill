////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math/preamble.hpp>
#include <math/vec/Vector2.hpp>

namespace yq {
    template <typename T>
    struct Circle2 {
        using component_t   = T;

        Vector2<T>  point;
        T           radius;
        
        constexpr bool operator==(const Circle2&) const noexcept = default;
    };

//  --------------------------------------------------------
//  COMPOSITION
    
    template <typename T>
    Circle2<T>  circle(const Vector2<T>& point, T radius)
    {
        return {point, radius};
    }

    YQ_NAN_1(Circle2, { nan_v<Vector2<T>>, nan_v<T> })
    YQ_ZERO_1(Circle2, { zero_v<Vector2<T>>, zero_v<T> })

//  --------------------------------------------------------
//  BASIC FUNCTIONS
    
    YQ_IS_FINITE_1(Circle2, is_finite(v.point) && is_finite(v.radius))
    YQ_IS_NAN_1(Circle2, is_nan(v.point) || is_nan(v.radius))

//  --------------------------------------------------------
//  ADVANCED FUNCTIONS

    template <typename T>
    square_t<T> area(const Circle2<T>& a)
    {
        return std::numbers::pi_v<ieee754_t<T>> * (a.radius*a.radius);
    }
    
    template <typename T>
    T           diameter(const Circle2<T>&a)
    {
        return a.radius + a.radius;
    }

    template <typename T>
    T   circumference(const Circle2<T>& a)
    {
        return ieee754_t<T>(2.) * std::numbers::pi_v<ieee754_t<T>> * a.radius;
    }

    template <typename T>
    T   permimeter(const Circle2<T>& a)
    {
        return ieee754_t<T>(2.) * std::numbers::pi_v<ieee754_t<T>> * a.radius;
    }
}

YQ_TYPE_DECLARE(yq::Circle2D)
YQ_TYPE_DECLARE(yq::Circle2F)
YQ_TYPE_DECLARE(yq::Circle2I)
YQ_TYPE_DECLARE(yq::Circle2U)

