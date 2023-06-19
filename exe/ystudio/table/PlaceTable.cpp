////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/place.hpp>
#include <mithril/atom/AtomCDB.hpp>
#include "PlaceTable.hpp"


PlaceTable::PlaceTable(QWidget*parent) : IdTableT<Place>(
    []() -> std::vector<Place> {
        return cdb::all_places(Sorted::YES);
    }, parent
)
{
    makeColumn<uint64_t> ("ID"sv, [](Place b) -> uint64_t { return b.id; });
    makeColumn<std::string> ("Name", [](Place b) -> std::string {
        return cdb::name(cdb::atom(b));
    });
}

PlaceTable::~PlaceTable()
{
}

#include "moc_PlaceTable.cpp"
