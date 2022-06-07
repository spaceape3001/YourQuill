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
#include <math/vec/Vec2.hpp>

namespace yq {
    
    /*! \brief Segment in 2 dimension(s)
    */
    template <typename T>
    struct Seg2 {
        using component_t   = T;

        Vec2<T>  a, b;

        constexpr bool operator==(const Seg2&) const noexcept = default;
    };

//  --------------------------------------------------------
//  COMPOSITION

    template <typename T>
    constexpr Seg2<T> segment(const Vec2<T>& a, const Vec2<T>& b)
    {
        return { a, b };
    }
    
    YQ_NAN_1(Seg2, { nan_v<Vec2>, nan_v<Vec2>});
    YQ_ZERO_1(Seg2, { zero_v<Vec2>, zero_v<Vec2>});

//  --------------------------------------------------------
//  BASIC FUNCTIONS

    YQ_IS_FINITE_1( Seg2, is_finite(v.a) && is_finite(v.b))
    YQ_IS_NAN_1(Seg2, is_nan(v.a) || is_nan(v.b))
    
//  --------------------------------------------------------
//  ADVANCED FUNCTIONS

    template <typename T>
    T       length(const Seg2<T>& seg)
    {
        return length(seg.b-seg.a);
    }
    
}
YQ_TYPE_DECLARE(yq::Seg2D)
YQ_TYPE_DECLARE(yq::Seg2F)
YQ_TYPE_DECLARE(yq::Seg2I)
YQ_TYPE_DECLARE(yq::Seg2U)

