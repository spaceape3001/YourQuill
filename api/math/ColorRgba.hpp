////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math/preamble.hpp>
#include <math/ColorRgb.hpp>

namespace yq {
    template <typename T>
    struct ColorRgba {
        using component_type = T;
    
        T   red;
        T   green;
        T   blue;
        T   alpha;
        
        constexpr bool    operator==(const ColorRgba&) const noexcept = default;
    };

//  --------------------------------------------------------
//  COMPOSITION

    /*! \brief Creates a $(N)d dimensioal vector
    
        Helper function to create a $(N) dimensional cartesian vector where the component type is deduced from
        the first argument.
    */
    template <typename T>
    constexpr ColorRgba<T> color(T r, std::type_identity_t<T> g, std::type_identity_t<T> b, std::type_identity_t<T> a)
    {
        return {r,g,b,a};
    }
    
    template <typename T>
    constexpr ColorRgba<T> color(const ColorRgb<T>& clr, std::type_identity_t<T> a)
    {
        return {clr.red,clr.green,clr.blue,a};
    }

    YQ_NAN_1(ColorRgba, {nan_v<T>, nan_v<T>, nan_v<T>, nan_v<T>})
    YQ_ZERO_1(ColorRgba, {zero_v<T>, zero_v<T>, zero_v<T>, zero_v<T>})
    
//  --------------------------------------------------------
//  BASIC FUNCTIONS

    YQ_IS_NAN_1(ColorRgba, is_nan(v.red) || is_nan(v.green) || is_nan(v.blue) || is_nan(v.alpha))
    YQ_IS_FINITE_1(ColorRgba, is_finite(v.red) && is_finite(v.green) && is_finite(v.blue) && is_finite(v.alpha))

}

YQ_TYPE_DECLARE(yq::ColorRgbaD)
YQ_TYPE_DECLARE(yq::ColorRgbaF)
YQ_TYPE_DECLARE(yq::ColorRgbaU8)
YQ_TYPE_DECLARE(yq::ColorRgbaU16)
