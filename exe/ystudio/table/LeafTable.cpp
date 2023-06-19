////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/leaf.hpp>
#include "LeafTable.hpp"


LeafTable::LeafTable(QWidget*parent) : IdTableT<Leaf>(
    []() -> std::vector<Leaf> {
        return cdb::all_leafs(Sorted::YES);
    }, parent
)
{
    makeColumn<std::string>("Key", [](Leaf l) -> std::string {
        return cdb::key(l);
    });
    makeColumn<std::string> ("Title", [](Leaf l) -> std::string {
        return cdb::title(l);
    });

    setWindowTitle(tr("Leafs"));
}

LeafTable::~LeafTable()
{
}

#include "moc_LeafTable.cpp"
