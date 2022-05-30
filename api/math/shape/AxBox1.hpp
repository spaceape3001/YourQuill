////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

//  WARNING!  WARNING!
//
//  THIS FILE IS AUTO-GENERATED, CHANGES WILL BE CLOBBERED!

#include <math/preamble.hpp>
#include <math/vec/Vec1.hpp>


namespace yq {

    template <typename T>
    struct AxBox1 {
        using component_t   = T;
        
        Vec1<T>  lo, hi;
        
        constexpr bool operator==(const AxBox1&) const noexcept = default;
    };
}

YQ_TYPE_DECLARE(yq::AxBox1D)
YQ_TYPE_DECLARE(yq::AxBox1F)
YQ_TYPE_DECLARE(yq::AxBox1I)
YQ_TYPE_DECLARE(yq::AxBox1U)

