////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <mithril/preamble.hpp>

namespace yq::mithril {
    /*! \brief Character in the cache database */
    struct Character {
        uint64_t            id = 0ULL;
        constexpr auto    operator<=>(const Character&rhs) const = default; 
        constexpr operator bool() const { return id != 0ULL; }
    };
}
