////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/event.hpp>
#include <mithril/atom/AtomCDB.hpp>
#include "EventTable.hpp"


EventTable::EventTable(QWidget*parent) : IdTableT<Event>(
    []() -> std::vector<Event> {
        return cdb::all_events(Sorted::YES);
    }, parent
)
{
    makeColumn<std::string>("Key", [](Event ev) -> std::string {
        return cdb::key(ev);
    });
    makeColumn<std::string> ("Title", [](Event ev) -> std::string {
        return cdb::title(ev);
    });
    setWindowTitle(tr("Events"));
}

EventTable::~EventTable()
{
}

#include "moc_EventTable.cpp"
