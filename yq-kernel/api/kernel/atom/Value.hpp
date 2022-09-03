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
    struct Value {
        struct Data;
        struct Info;
        struct ListData;
        struct ListFile;

        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Value&rhs) const = default;
        constexpr operator bool() const { return id != 0ULL; }
    };
}

