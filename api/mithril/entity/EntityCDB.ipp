////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "EntityCDB.hpp"
#include <mithril/atom/AtomCDB.hpp>
#include <mithril/image/Image.hpp>

namespace yq::mithril::cdb {
    std::vector<Entity>     all_entities(Sorted sorted)
    {
        return blind_remap(all_atoms(entity_class(), sorted), Entity());
    }
    
    Atom                    atom(Entity e)
    {
        return Atom( e.id );
    }
    
    Entity                  entity(Atom a)
    {
        return exists_entity(a.id) ? Entity(a.id) : Entity();
    }
    
    Entity                  entity(std::string_view k)
    {
        return entity(atom(k)); // TODO better name lookups
    }
    
    Entity                  entity(uint64_t i)
    {
        return exists_entity(i) ? Entity{i} : Entity{};
    }

    Class                   entity_class()
    {
        static const Class  cls = db_class("Entity");
        return cls;
    }
    
    bool                    exists(Entity e)
    {
        return exists_entity(e.id);
    }
    
    bool                    exists_entity(uint64_t i)
    {
        return is(Atom(i), entity_class());
    }
    
    Image                   icon(Entity e)
    {
        return icon(atom(e));
    }
    
    std::string             key(Entity e)
    {
        return key(atom(e));
    }
    
    std::string             name(Entity e)
    {
        return name(atom(e));
    }
}
