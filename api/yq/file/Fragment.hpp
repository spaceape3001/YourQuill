////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <compare>


namespace yq {

    /*! \brief Represents single fragment
    */
    struct Fragment {
        struct Info;
        uint64_t  id = 0ULL;
        constexpr auto    operator<=>(const Fragment& rhs) const = default;
        constexpr operator bool() const { return id != 0ULL; }
    };
}
