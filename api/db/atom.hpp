////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <compare>
#include <cstdint>

namespace yq {

    /*! \brief Atom in the cache
        This structure represents an atom in the cache
    */
    struct Atom {
        struct Info;
        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Atom&rhs) const = default; 
        constexpr operator bool() const { return id != 0ULL; }
    };

}
