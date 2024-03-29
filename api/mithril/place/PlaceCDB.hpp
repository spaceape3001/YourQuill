////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>
#include <mithril/place/Place.hpp>

namespace yq::mithril::cdb {
    std::vector<Place>  all_places(Sorted sorted=Sorted());
    Atom                atom(Place);
    bool                exists(Place);
    bool                exists_place(uint64_t);
    Image               icon(Place);
    std::string         key(Place);
    std::string         name(Place);
    Place               place(Atom);
    Class               place_class();
}
