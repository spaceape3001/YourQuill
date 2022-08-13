////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <compare>
#include <cstdint>
#include <initializer_list>
#include <string_view>

namespace yq {
    template <typename> class IDLock;

    /*! \brief Atom in the cache
        This structure represents an atom in the cache
    */
    struct Atom {
        struct Info;
        using Lock = IDLock<Atom>;
        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Atom&rhs) const = default; 
        constexpr operator bool() const { return id != 0ULL; }
    };

}
