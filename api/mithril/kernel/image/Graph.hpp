////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <compare>
#include <cstdint>

namespace yq {
    namespace mithril {
        struct Graph {
            uint64_t  id = 0ULL;
            constexpr auto    operator<=>(const Graph& rhs) const noexcept = default;
            constexpr operator uint64_t () const noexcept { return id; }
        };
    }
}
