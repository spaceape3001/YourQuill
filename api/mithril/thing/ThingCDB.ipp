////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/AtomCDB.hpp>
#include <mithril/class/ClassCDB.hpp>
#include <mithril/thing/ThingCDB.hpp>

namespace yq::mithril::cdb {
    std::vector<Thing>      all_things(Sorted sorted)
    {
        return blind_remap(all_atoms(thing_class(), sorted), Thing());
    }
    
    Atom  atom(Thing th)
    {
        return Atom{ th.id };
    }

    bool  exists(Thing th)
    {
        return exists_thing(th.id);
    }
    
    bool  exists_thing(uint64_t i)
    {
        return is(Atom(i), thing_class());
    }
    
    Image               icon(Thing t)
    {
        return image(atom(t));
    }
    
    std::string         key(Thing t)
    {
        return key(atom(t));
    }
    
    std::string         name(Thing t)
    {
        return name(atom(t));
    }
    
    Thing  thing(Atom at)
    {
        return exists_thing(at.id) ? Thing(at.id) : Thing();
    }

    Class  thing_class()
    {
        static const Class  cls = db_class("Thing");
        return cls;
    }

}
