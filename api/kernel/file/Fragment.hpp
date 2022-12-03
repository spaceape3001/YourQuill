////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/preamble.hpp>

namespace yq {

    /*! \brief Represents single fragment
    */
    struct Fragment {
        struct Info;
        using Lock = IDLock<Fragment>;
        uint64_t  id = 0ULL;
        constexpr auto    operator<=>(const Fragment& rhs) const noexcept = default;
        constexpr operator uint64_t() const noexcept { return id != 0ULL; }
    };
}

YQ_TYPE_DECLARE(yq::Fragment)
