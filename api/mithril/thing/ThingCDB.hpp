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
        Thing               thing(Atom);
        Class               thing_class();
        bool                exists(Thing);
        bool                exists_thing(uint64_t);
    }
}
