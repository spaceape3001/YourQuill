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
        Event               event(std::string_view);
        Event               event(uint64_t);
        Class               event_class();
        bool                exists(Event);
        bool                exists_event(uint64_t);
        Image               icon(Event);
        std::string         key(Event);
        std::string         name(Event);
        std::string         title(Event);
    }
}
