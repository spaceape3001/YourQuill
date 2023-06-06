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
    
    Atom  atom(Thing ch)
    {
        return Atom{ ch.id };
    }

    Thing  thing(Atom at)
    {
        return is(at, thing_class()) ? Thing(at.id) : Thing();
    }

    Class  thing_class()
    {
        static const Class  cls = db_class("Thing");
        return cls;
    }

    bool  exists(Thing ch)
    {
        return is(Atom(ch.id), thing_class());
    }
    
}
