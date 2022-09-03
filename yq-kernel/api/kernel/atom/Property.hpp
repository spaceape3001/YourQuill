////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <compare>
#include <cstdint>

namespace yq {
    /*! \brief Predefined value for a field
    
        This is a predefined attribute value for a field
    */
    struct Property {

        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Property&rhs) const = default;
        constexpr operator bool() const { return id != 0ULL; }
    };
}

