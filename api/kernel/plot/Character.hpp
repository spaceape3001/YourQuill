////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <kernel/preamble.hpp>

namespace yq {

    /*! \brief Character in the cache database */
    struct Character {
        uint64_t            id = 0ULL;
        constexpr auto    operator<=>(const Character&rhs) const = default; 
        constexpr operator bool() const { return id != 0ULL; }
    };
}
