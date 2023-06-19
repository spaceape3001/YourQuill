////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/field.hpp>
#include "FieldTable.hpp"


FieldTable::FieldTable(QWidget*parent) : IdTableT<Field>(
    []() -> std::vector<Field> {
        return cdb::all_fields(Sorted::YES);
    }, parent
)
{
    makeColumn<std::string>("Key", [](Field f) -> std::string {
        return cdb::key(f);
    });
    makeColumn<std::string> ("Name", [](Field f) -> std::string {
        return cdb::name(f);
    });

    setWindowTitle(tr("Fields"));
}

FieldTable::~FieldTable()
{
}

#include "moc_FieldTable.cpp"
