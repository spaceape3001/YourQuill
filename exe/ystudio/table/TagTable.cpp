////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/tag.hpp>
#include "TagTable.hpp"


TagTable::TagTable(QWidget*parent) : IdTableT<Tag>(
    []() -> std::vector<Tag> {
        return cdb::all_tags(Sorted::YES);
    }, parent
)
{
    makeColumn<std::string>("Key", [](Tag t) -> std::string {
        return cdb::key(t);
    });
    makeColumn<std::string> ("Name", [](Tag t) -> std::string {
        return cdb::name(t);
    });
    setWindowTitle(tr("Tags"));
}

TagTable::~TagTable()
{
}

#include "moc_TagTable.cpp"
