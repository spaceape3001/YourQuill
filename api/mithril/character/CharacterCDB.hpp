////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>
#include <mithril/character/Character.hpp>

namespace yq::mithril {        
    namespace cdb {
        std::vector<Character>      all_characters(Sorted sorted=Sorted());
        Atom                        atom(Character);
        Character                   character(Atom);
        Class                       character_class();
        bool                        exists(Character);
        bool                        exists_character(uint64_t);
        std::string                 key(Character);
        std::string                 name(Character);
    }
}
