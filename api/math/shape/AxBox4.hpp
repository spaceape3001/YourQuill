////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

//  WARNING!  WARNING!
//
//  THIS FILE IS AUTO-GENERATED, CHANGES WILL BE CLOBBERED!

#include <math/preamble.hpp>
#include <math/vec/Vec4.hpp>


namespace yq {

    template <typename T>
    struct AxBox4 {
        using component_t   = T;
        
        Vec4<T>  lo, hi;
        
        constexpr bool operator==(const AxBox4&) const noexcept = default;
    };
}

YQ_TYPE_DECLARE(yq::AxBox4D)
YQ_TYPE_DECLARE(yq::AxBox4F)
YQ_TYPE_DECLARE(yq::AxBox4I)
YQ_TYPE_DECLARE(yq::AxBox4U)

