////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <compare>
#include <cstdint>

namespace yq {
    struct Leaf {
        
        static constexpr const char*    szExtension = "y";
        struct Info;
        struct Merge;
        struct Data;
        struct File;
        
        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Leaf&rhs) const = default;
        constexpr operator bool() const { return id != 0ULL; }
    };
}
