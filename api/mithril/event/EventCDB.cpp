////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/atom/AtomCDB.hpp>
#include <mithril/class/ClassCDB.hpp>
#include <mithril/event/EventCDB.hpp>
#include <mithril/logging.hpp>

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

    Event   event(std::string_view k)
    {
        return event(atom(k));  // TODO -- better lookups
    }
    
    Event   event(uint64_t i)
    {
        return exists_event(i) ? Event(i) : Event();
    }

    Class  event_class()
    {
        static const Class  cls = db_class("Event");
        return cls;
    }

    bool  exists(Event evt)
    {
        return exists_event(evt.id);
    }

    bool  exists_event(uint64_t i)
    {
        return is(Atom(i), event_class());
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
