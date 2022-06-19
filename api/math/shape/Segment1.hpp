////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <math/preamble.hpp>
#include <math/vec/Vector1.hpp>
#include "SegmentData.hpp"

namespace yq {
    

//  --------------------------------------------------------
//  COMPOSITION

    template <typename T>
    constexpr Segment1<T> segment(const Vector1<T>& a, const Vector1<T>& b)
    {
        return { a, b };
    }
    
    YQ_NAN_1(Segment1, { nan_v<Vector1<T>>, nan_v<Vector1<T>>});
    YQ_ZERO_1(Segment1, { zero_v<Vector1<T>>, zero_v<Vector1<T>>});

//  --------------------------------------------------------
//  BASIC FUNCTIONS

    YQ_IS_FINITE_1( Segment1, is_finite(v.a) && is_finite(v.b))
    YQ_IS_NAN_1(Segment1, is_nan(v.a) || is_nan(v.b))
    
//  --------------------------------------------------------
//  ADVANCED FUNCTIONS


    template <typename T>
    T       length(const Segment1<T>& seg)
    {
        return length(seg.b-seg.a);
    }
    
    template <typename T>
    requires has_ieee754_v<T>
    Vector1<T>     midpoint(const Segment1<T>& seg)
    {
        return ieee754_t<T>(0.5)*(seg.hi+seg.lo);
    }

    template <typename T>
    requires has_ieee754_v<T>
    Vector1<T>     point(const Segment1<T>& seg, ieee754_t<T> f)
    {
        return (one_v<ieee754_t<T>> - f) * seg.a + f * seg.b;
    }
}
YQ_TYPE_DECLARE(yq::Segment1D)
YQ_TYPE_DECLARE(yq::Segment1F)
YQ_TYPE_DECLARE(yq::Segment1I)
YQ_TYPE_DECLARE(yq::Segment1U)

