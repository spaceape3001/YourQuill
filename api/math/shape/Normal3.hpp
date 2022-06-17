////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math/preamble.hpp>
#include <math/vec/Vector3.hpp>

namespace yq {
    template <typename T>
    struct Normal3 {
        using component_type    = T;
        Vector3<T>              direction;
        
        constexpr bool operator==(const Normal3& rhs) const noexcept = default;
    };
    
    YQ_IEEE754_1(Normal3)
        
//  --------------------------------------------------------
//  COMPOSITION
    
    template <typename T>
    Normal3<T>     normal(const Vector3<T>& dir) 
    {
        return { ~dir };
    }
    
    template <typename T>
    requires std::is_floating_point_v<T>
    Normal3<T>     normal(T x, std::type_identity_t<T> y, std::type_identity_t<T> z)
    {
        return { ~Vector3<T>{x,y,z} };
    }
    
    YQ_NAN_1(Normal3, { nan_v<Vector3<T>> })
    YQ_ZERO_1(Normal3, { zero_v<Vector3<T>> })
    
//  --------------------------------------------------------
//  BASIC FUNCTIONS

    YQ_IS_NAN_1(Normal3, is_nan(v.direction))
    YQ_IS_FINITE_1(Normal3, is_finite(v.direction))
}

YQ_TYPE_DECLARE(yq::Normal3D)
YQ_TYPE_DECLARE(yq::Normal3F)
