////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Entity.hpp"

namespace yq::mithril::cdb {
    std::vector<Entity>     all_entities(Sorted sorted=Sorted());
    Atom                    atom(Entity);
    Entity                  entity(Atom);
    Entity                  entity(std::string_view);
    Entity                  entity(uint64_t);
    Class                   entity_class();
    bool                    exists(Entity);
    bool                    exists_entity(uint64_t);
    Image                   icon(Entity);
    std::string             key(Entity);
    std::string             name(Entity);
}

