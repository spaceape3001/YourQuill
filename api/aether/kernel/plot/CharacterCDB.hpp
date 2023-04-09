////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <aether/kernel/preamble.hpp>
#include <aether/kernel/plot/Character.hpp>

namespace yq {
    namespace cdb {
        std::vector<Character>      all_characters(Sorted sorted=Sorted());
        Atom                        atom(Character);
        Character                   character(Atom);
        Class                       character_class();
        bool                        exists(Character);
        bool                        exists_character(uint64_t);
    }
}
