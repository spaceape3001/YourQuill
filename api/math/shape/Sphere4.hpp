////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math/preamble.hpp>
#include <math/vec/Vector4.hpp>

namespace yq {

    /*! \brief Hypersphere in four dimensions
    */
    template <typename T>
    struct Sphere4 {
        using component_t   = T;

        Vector4<T>     pt;
        T           r;
        
        constexpr bool operator==(const Sphere4&) const noexcept = default;
    };

//  --------------------------------------------------------
//  COMPOSITION
    
    template <typename T>
    Sphere4<T>  sphere(const Vector4<T>& pt, T r)
    {
        return {pt, r};
    }

    YQ_NAN_1(Sphere4, { nan_v<Vector4<T>>, nan_v<T> })
    YQ_ZERO_1(Sphere4, { zero_v<Vector4<T>>, zero_v<T> })

//  --------------------------------------------------------
//  BASIC FUNCTIONS
    
    YQ_IS_FINITE_1(Sphere4, is_finite(v.pt) && is_finite(v.r))
    YQ_IS_NAN_1(Sphere4, is_nan(v.pt) || is_nan(v.r))

//  --------------------------------------------------------
//  ADVANCED FUNCTIONS

    template <typename T>
    T           diameter(const Sphere4<T>&a)
    {
        return a.r + a.r;
    }

}

YQ_TYPE_DECLARE(yq::Sphere4D)
YQ_TYPE_DECLARE(yq::Sphere4F)
YQ_TYPE_DECLARE(yq::Sphere4I)
YQ_TYPE_DECLARE(yq::Sphere4U)

