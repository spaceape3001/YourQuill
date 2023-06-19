////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/place.hpp>
#include "PlaceTable.hpp"


PlaceTable::PlaceTable(QWidget*parent) : IdTableT<Place>(
    []() -> std::vector<Place> {
        return cdb::all_places(Sorted::YES);
    }, parent
)
{
    makeColumn<std::string>("Key", [](Place p) -> std::string {
        return cdb::key(p);
    });
    makeColumn<std::string> ("Name", [](Place p) -> std::string {
        return cdb::name(p);
    });
    setWindowTitle(tr("Places"));
}

PlaceTable::~PlaceTable()
{
}

#include "moc_PlaceTable.cpp"
