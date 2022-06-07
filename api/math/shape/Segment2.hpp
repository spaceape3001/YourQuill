////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <math/preamble.hpp>
#include <math/vec/Vector2.hpp>

namespace yq {
    
    /*! \brief Segment in 2 dimension(s)
    */
    template <typename T>
    struct Segment2 {
        using component_t   = T;

        Vector2<T>  a, b;

        constexpr bool operator==(const Segment2&) const noexcept = default;
    };

//  --------------------------------------------------------
//  COMPOSITION

    template <typename T>
    constexpr Segment2<T> segment(const Vector2<T>& a, const Vector2<T>& b)
    {
        return { a, b };
    }
    
    YQ_NAN_1(Segment2, { nan_v<Vector2<T>>, nan_v<Vector2<T>>});
    YQ_ZERO_1(Segment2, { zero_v<Vector2<T>>, zero_v<Vector2<T>>});

//  --------------------------------------------------------
//  BASIC FUNCTIONS

    YQ_IS_FINITE_1( Segment2, is_finite(v.a) && is_finite(v.b))
    YQ_IS_NAN_1(Segment2, is_nan(v.a) || is_nan(v.b))
    
//  --------------------------------------------------------
//  ADVANCED FUNCTIONS

    template <typename T>
    T       length(const Segment2<T>& seg)
    {
        return length(seg.b-seg.a);
    }

    template <typename T>
    requires has_ieee754_v<T>
    Vector2<T>     midpoint(const Segment2<T>& seg)
    {
        return ieee754_t<T>(0.5)*(seg.hi+seg.lo);
    }
    
    template <typename T>
    requires has_ieee754_v<T>
    Vector2<T>     point(const Segment2<T>& seg, ieee754_t<T> f)
    {
        return (one_v<ieee754_t<T>> - f) * seg.a + f * seg.b;
    }
}

YQ_TYPE_DECLARE(yq::Segment2D)
YQ_TYPE_DECLARE(yq::Segment2F)
YQ_TYPE_DECLARE(yq::Segment2I)
YQ_TYPE_DECLARE(yq::Segment2U)

