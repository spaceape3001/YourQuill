////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

//  WARNING!  WARNING!
//
//  THIS FILE IS AUTO-GENERATED, CHANGES WILL BE CLOBBERED!

#include <math/preamble.hpp>
#include <math/vec/Vec3.hpp>


namespace yq {

    template <typename T>
    struct AxBox3 {
        using component_t   = T;
        
        Vec3<T>  lo, hi;
        
        constexpr bool operator==(const AxBox3&) const noexcept = default;
    };
}

YQ_TYPE_DECLARE(yq::AxBox3D)
YQ_TYPE_DECLARE(yq::AxBox3F)
YQ_TYPE_DECLARE(yq::AxBox3I)
YQ_TYPE_DECLARE(yq::AxBox3U)

