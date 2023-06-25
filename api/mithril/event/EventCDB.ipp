////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/AtomCDB.hpp>
#include <mithril/class/ClassCDB.hpp>
#include <mithril/event/EventCDB.hpp>

namespace yq::mithril::cdb {
    std::vector<Event>  all_events(Sorted sorted)
    {
        return blind_remap(all_atoms(event_class(), sorted), Event());
    }
    
    Atom  atom(Event ch)
    {
        return Atom{ ch.id };
    }

    Event  event(Atom at)
    {
        return is(at, event_class()) ? Event(at.id) : Event();
    }

    Class  event_class()
    {
        static const Class  cls = db_class("Event");
        return cls;
    }

    bool  exists(Event ch)
    {
        return is(Atom(ch.id), event_class());
    }

    Image  icon(Event evt)
    {
        return Image(atom(evt));
    }

    std::string  key(Event evt)
    {
        return key(atom(evt));
    }
    
    std::string  name(Event evt)
    {
        return name(atom(evt));
    }

    std::string  title(Event evt)
    {
        return name(atom(evt));
    }
}
