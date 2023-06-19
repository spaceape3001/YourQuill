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
    makeColumn<uint64_t> ("ID"sv, [](Event b) -> uint64_t { return b.id; });
    makeColumn<std::string> ("Name", [](Event b) -> std::string {
        return cdb::name(cdb::atom(b));
    });
}

EventTable::~EventTable()
{
}

#include "moc_EventTable.cpp"
