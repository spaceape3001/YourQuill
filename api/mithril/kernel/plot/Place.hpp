#pragma once

#include "PlotFwd.hpp"

namespace yq {
    namespace mithril {
        struct Place {
            uint64_t            id = 0ULL;
            constexpr auto    operator<=>(const Place&rhs) const = default; 
            constexpr operator bool() const { return id != 0ULL; }
        };


        namespace cdb {
        }
    }
}
