////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <compare>
#include <cstdint>

namespace yq {
    /*! \brief Property binds attributes to atoms
    
    */
    struct Property {
        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Property&rhs) const = default;
        constexpr operator uint64_t () const { return id; }
    };
}
