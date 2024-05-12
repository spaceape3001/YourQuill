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

    bool  exists(Place ch)
    {
        return is(Atom(ch.id), place_class());
    }

    bool  exists_place(uint64_t id)
    {
        return is(Atom(id), place_class());
    }

    Image               icon(Place p)
    {
        return icon(atom(p));
    }

    std::string         key(Place p)
    {
        return key(atom(p));
    }
    
    std::string         name(Place p)
    {
        return name(atom(p));
    }

    Place  place(Atom at)
    {
        return is(at, place_class()) ? Place(at.id) : Place();
    }
    
    Place               place(std::string_view k)
    {
        return place(atom(k));  // TODO -- better lookup
    }
    
    Place               place(uint64_t id)
    {
        return exists_place(id) ? Place(id) : Place();
    }


    Class  place_class()
    {
        static const Class  cls = db_class("Place");
        return cls;
    }

}
