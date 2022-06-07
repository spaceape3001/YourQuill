////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <math/preamble.hpp>
#include <math/vec/Vec3.hpp>

namespace yq {
    
    /*! \brief Segment in 3 dimension(s)
    */
    template <typename T>
    struct Seg3 {
        using component_t   = T;

        Vec3<T>  a, b;

        constexpr bool operator==(const Seg3&) const noexcept = default;
    };

//  --------------------------------------------------------
//  COMPOSITION

    template <typename T>
    constexpr Seg3<T> segment(const Vec3<T>& a, const Vec3<T>& b)
    {
        return { a, b };
    }
    
    YQ_NAN_1(Seg3, { nan_v<Vec3>, nan_v<Vec3>});
    YQ_ZERO_1(Seg3, { zero_v<Vec3>, zero_v<Vec3>});

//  --------------------------------------------------------
//  BASIC FUNCTIONS

    YQ_IS_FINITE_1( Seg3, is_finite(v.a) && is_finite(v.b))
    YQ_IS_NAN_1(Seg3, is_nan(v.a) || is_nan(v.b))
    
//  --------------------------------------------------------
//  ADVANCED FUNCTIONS

    template <typename T>
    T       length(const Seg3<T>& seg)
    {
        return length(seg.b-seg.a);
    }
    
    template <typename T>
    requires has_ieee754_v<T>
    Vec3<T>     midpoint(const Seg3<T>& seg)
    {
        return ieee754_t<T>(0.5)*(seg.hi+seg.lo);
    }

    template <typename T>
    requires has_ieee754_v<T>
    Vec3<T>     point(const Seg3<T>& seg, ieee754_t<T> f)
    {
        return (one_v<ieee754_t<T>> - f) * seg.a + f * seg.b;
    }
    
}

YQ_TYPE_DECLARE(yq::Seg3D)
YQ_TYPE_DECLARE(yq::Seg3F)
YQ_TYPE_DECLARE(yq::Seg3I)
YQ_TYPE_DECLARE(yq::Seg3U)

