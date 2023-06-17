////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/AtomCDB.hpp>
#include <mithril/class/ClassCDB.hpp>
#include <mithril/place/PlaceCDB.hpp>

namespace yq::mithril::cdb {
    std::vector<Place>      all_places(Sorted sorted)
    {
        return blind_remap(all_atoms(place_class(), sorted), Place());
    }
    
    Atom  atom(Place ch)
    {
        return Atom{ ch.id };
    }

    Place  place(Atom at)
    {
        return is(at, place_class()) ? Place(at.id) : Place();
    }

    Class  place_class()
    {
        static const Class  cls = db_class("Place");
        return cls;
    }

    bool  exists(Place ch)
    {
        return is(Atom(ch.id), place_class());
    }
    
}