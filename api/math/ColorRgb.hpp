////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math/preamble.hpp>

namespace yq {
    template <typename T>
    struct ColorRgb {
        using component_type = T;
    
        T   red;
        T   green;
        T   blue;
        T   alpha;
        
        constexpr bool    operator==(const ColorRgb&) const noexcept = default;
    };

//  --------------------------------------------------------
//  COMPOSITION

    /*! \brief Creates a $(N)d dimensioal vector
    
        Helper function to create a $(N) dimensional cartesian vector where the component type is deduced from
        the first argument.
    */
    template <typename T>
    constexpr ColorRgb<T> color(T r, std::type_identity_t<T> g, std::type_identity_t<T> b)
    {
        return {r,g,b};
    }
    
    YQ_NAN_1(ColorRgb, {nan_v<T>, nan_v<T>, nan_v<T>})
    YQ_ZERO_1(ColorRgb, {zero_v<T>, zero_v<T>, zero_v<T>})
    
//  --------------------------------------------------------
//  BASIC FUNCTIONS

    YQ_IS_NAN_1(ColorRgb, is_nan(v.red) || is_nan(v.green) || is_nan(v.blue))
    YQ_IS_FINITE_1(ColorRgb, is_finite(v.red) && is_finite(v.green) && is_finite(v.blue))
    
}

YQ_TYPE_DECLARE(yq::ColorRgbD)
YQ_TYPE_DECLARE(yq::ColorRgbF)
YQ_TYPE_DECLARE(yq::ColorRgbU8)
YQ_TYPE_DECLARE(yq::ColorRgbU16)
