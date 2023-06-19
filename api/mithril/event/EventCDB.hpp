////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>
#include <mithril/event/Event.hpp>

namespace yq::mithril {
    namespace cdb {
        std::vector<Event>  all_events(Sorted sorted=Sorted());
        Atom                atom(Event);
        Event               event(Atom);
        Class               event_class();
        bool                exists(Event);
        bool                exists_event(uint64_t);
        std::string         key(Event);
        std::string         title(Event);
    }
}
