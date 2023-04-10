#pragma once

#include <mithril/yq/CacheFwd.hpp>

namespace yq {
    namespace mithril {

        struct Character;
        struct Group;
        struct Place;
        struct Event;

        using CharSet   = Set<Character>;
        using EventSet  = Set<Event>;
        using GroupSet  = Set<Group>;
        using PlaceSet  = Set<Place>;

        using CharVec   = Vector<Character>;
        using EventVec  = Vector<Event>;
        using GroupVec  = Vector<Group>;
        using PlaceVec  = Vector<Place>;


        namespace cdb {
        }
    }
}
