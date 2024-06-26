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
        Character                   character(Entity);
        Character                   character(std::string_view);
        Character                   character(uint64_t);
        Class                       character_class();
        Entity                      entity(Character);
        bool                        exists(Character);
        bool                        exists_character(uint64_t);
        Image                       icon(Character);
        std::string                 key(Character);
        std::string                 name(Character);
    }
}
