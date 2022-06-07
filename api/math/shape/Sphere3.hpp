////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math/preamble.hpp>
#include <math/vec/Vec3.hpp>

namespace yq {
    template <typename T>
    struct Sphere3 {
        using component_t   = T;

        Vec3<T>     pt;
        T           r;
        
        constexpr bool operator==(const Sphere3&) const noexcept = default;
    };

//  --------------------------------------------------------
//  COMPOSITION
    
    template <typename T>
    Sphere3<T>  sphere(const Vec3<T>& pt, T r)
    {
        return {pt, r};
    }

    YQ_NAN_1(Sphere3, { nan_v<Vec3<T>>, nan_v<T> })
    YQ_ZERO_1(Sphere3, { zero_v<Vec3<T>>, zero_v<T> })

//  --------------------------------------------------------
//  BASIC FUNCTIONS
    
    YQ_IS_FINITE_1(Sphere3, is_finite(v.pt) && is_finite(v.r))
    YQ_IS_NAN_1(Sphere3, is_nan(v.pt) || is_nan(v.r))

//  --------------------------------------------------------
//  ADVANCED FUNCTIONS

    template <typename T>
    T           diameter(const Sphere3<T>&a)
    {
        return a.r + a.r;
    }
}

YQ_TYPE_DECLARE(yq::Sphere3D)
YQ_TYPE_DECLARE(yq::Sphere3F)
YQ_TYPE_DECLARE(yq::Sphere3I)
YQ_TYPE_DECLARE(yq::Sphere3U)

