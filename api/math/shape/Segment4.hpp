////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <math/preamble.hpp>
#include <math/vec/Vector4.hpp>

namespace yq {
    
    /*! \brief Segment in 4 dimension(s)
    */
    template <typename T>
    struct Segment4 {
        using component_t   = T;

        Vector4<T>  a, b;

        constexpr bool operator==(const Segment4&) const noexcept = default;
    };

//  --------------------------------------------------------
//  COMPOSITION

    template <typename T>
    constexpr Segment4<T> segment(const Vector4<T>& a, const Vector4<T>& b)
    {
        return { a, b };
    }
    
    YQ_NAN_1(Segment4, { nan_v<Vector4<T>>, nan_v<Vector4<T>>});
    YQ_ZERO_1(Segment4, { zero_v<Vector4<T>>, zero_v<Vector4<T>>});

//  --------------------------------------------------------
//  BASIC FUNCTIONS

    YQ_IS_FINITE_1( Segment4, is_finite(v.a) && is_finite(v.b))
    YQ_IS_NAN_1(Segment4, is_nan(v.a) || is_nan(v.b))
    
//  --------------------------------------------------------
//  ADVANCED FUNCTIONS

    template <typename T>
    T       length(const Segment4<T>& seg)
    {
        return length(seg.b-seg.a);
    }

    template <typename T>
    requires has_ieee754_v<T>
    Vector4<T>     midpoint(const Segment4<T>& seg)
    {
        return ieee754_t<T>(0.5)*(seg.hi+seg.lo);
    }

    template <typename T>
    requires has_ieee754_v<T>
    Vector4<T>     point(const Segment4<T>& seg, ieee754_t<T> f)
    {
        return (one_v<ieee754_t<T>> - f) * seg.a + f * seg.b;
    }
    
}
YQ_TYPE_DECLARE(yq::Segment4D)
YQ_TYPE_DECLARE(yq::Segment4F)
YQ_TYPE_DECLARE(yq::Segment4I)
YQ_TYPE_DECLARE(yq::Segment4U)

