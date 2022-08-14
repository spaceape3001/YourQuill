#pragma once

#include <cstdint>
#include "PlotFwd.hpp"

struct Atom;

struct Character {
    uint64_t            id = 0ULL;
    constexpr auto    operator<=>(const Character&rhs) const = default; 
    constexpr operator bool() const { return id != 0ULL; }
};

namespace cdb {
    Vector<Character>   all_characters();

    Character           character(Atom);
    Character           db_character(Atom);
}

