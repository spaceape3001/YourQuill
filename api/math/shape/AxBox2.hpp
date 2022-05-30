////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

//  WARNING!  WARNING!
//
//  THIS FILE IS AUTO-GENERATED, CHANGES WILL BE CLOBBERED!

#include <math/preamble.hpp>
#include <math/vec/Vec2.hpp>


namespace yq {

    template <typename T>
    struct AxBox2 {
        using component_t   = T;
        
        Vec2<T>  lo, hi;
        
        constexpr bool operator==(const AxBox2&) const noexcept = default;
    };
}

YQ_TYPE_DECLARE(yq::AxBox2D)
YQ_TYPE_DECLARE(yq::AxBox2F)
YQ_TYPE_DECLARE(yq::AxBox2I)
YQ_TYPE_DECLARE(yq::AxBox2U)

