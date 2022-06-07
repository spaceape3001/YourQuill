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
#include <math/vec/Vec4.hpp>

namespace yq {
    
    /*! \brief Segment in 4 dimension(s)
    */
    template <typename T>
    struct Seg4 {
        using component_t   = T;

        Vec4<T>  a, b;

        constexpr bool operator==(const Seg4&) const noexcept = default;
    };

//  --------------------------------------------------------
//  COMPOSITION

    template <typename T>
    constexpr Seg4<T> segment(const Vec4<T>& a, const Vec4<T>& b)
    {
        return { a, b };
    }
    
    YQ_NAN_1(Seg4, { nan_v<Vec4>, nan_v<Vec4>});
    YQ_ZERO_1(Seg4, { zero_v<Vec4>, zero_v<Vec4>});

//  --------------------------------------------------------
//  BASIC FUNCTIONS

    YQ_IS_FINITE_1( Seg4, is_finite(v.a) && is_finite(v.b))
    YQ_IS_NAN_1(Seg4, is_nan(v.a) || is_nan(v.b))
    
//  --------------------------------------------------------
//  ADVANCED FUNCTIONS

    template <typename T>
    T       length(const Seg4<T>& seg)
    {
        return length(seg.b-seg.a);
    }
    
}
YQ_TYPE_DECLARE(yq::Seg4D)
YQ_TYPE_DECLARE(yq::Seg4F)
YQ_TYPE_DECLARE(yq::Seg4I)
YQ_TYPE_DECLARE(yq::Seg4U)

