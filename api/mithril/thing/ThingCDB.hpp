////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>
#include <mithril/thing/Thing.hpp>

namespace yq::mithril {        
    namespace cdb {
        std::vector<Thing>  all_things(Sorted sorted=Sorted());
        Atom                atom(Thing);
        bool                exists(Thing);
        bool                exists_thing(uint64_t);
        Image               icon(Thing);
        std::string         key(Thing);
        std::string         name(Thing);
        Thing               thing(Atom);
        Class               thing_class();
    }
}
