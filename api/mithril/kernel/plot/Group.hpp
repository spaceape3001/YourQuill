#pragma once

#include "PlotFwd.hpp"

namespace yq {
    namespace mithril {
        struct Group {
            uint64_t            id = 0ULL;
            constexpr auto    operator<=>(const Group&rhs) const = default; 
            constexpr operator bool() const { return id != 0ULL; }
        };


        namespace cdb {
        }
    }
}
