#pragma once

#include "PlotFwd.hpp"

struct Event {
    uint64_t            id = 0ULL;
    constexpr auto    operator<=>(const Event&rhs) const = default; 
    constexpr operator bool() const { return id != 0ULL; }
};


namespace cdb {
    Event           db_event(Atom);
    Event           event(Atom);
}
