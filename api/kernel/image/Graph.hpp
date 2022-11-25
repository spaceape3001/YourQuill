////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <compare>
#include <cstdint>

namespace yq {
    struct Graph {
        uint64_t  id = 0ULL;
        constexpr auto    operator<=>(const Graph& rhs) const = default;
        constexpr operator bool() const { return id != 0ULL; }
    };
}
