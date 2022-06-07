////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

//  WARNING!  WARNING!
//
//  THIS FILE IS AUTO-GENERATED, CHANGES WILL BE CLOBBERED!

#include <math/preamble.hpp>
#include <math/vec/Vec1.hpp>

namespace yq {
    
    /*! \brief Segment in 1 dimension(s)
    */
    template <typename T>
    struct Seg1 {
        using component_t   = T;

        Vec1<T>  a, b;

        constexpr bool operator==(const Seg1&) const noexcept = default;
    };

//  --------------------------------------------------------
//  COMPOSITION

    template <typename T>
    constexpr Seg1<T> segment(const Vec1<T>& a, const Vec1<T>& b)
    {
        return { a, b };
    }
    
    YQ_NAN_1(Seg1, { nan_v<Vec1>, nan_v<Vec1>});
    YQ_ZERO_1(Seg1, { zero_v<Vec1>, zero_v<Vec1>});

//  --------------------------------------------------------
//  BASIC FUNCTIONS

    YQ_IS_FINITE_1( Seg1, is_finite(v.a) && is_finite(v.b))
    YQ_IS_NAN_1(Seg1, is_nan(v.a) || is_nan(v.b))
    
//  --------------------------------------------------------
//  ADVANCED FUNCTIONS

    template <typename T>
    T       length(const Seg1<T>& seg)
    {
        return length(seg.b-seg.a);
    }
    
}
YQ_TYPE_DECLARE(yq::Seg1D)
YQ_TYPE_DECLARE(yq::Seg1F)
YQ_TYPE_DECLARE(yq::Seg1I)
YQ_TYPE_DECLARE(yq::Seg1U)

