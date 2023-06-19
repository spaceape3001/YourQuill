////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/class.hpp>
#include "ClassTable.hpp"


ClassTable::ClassTable(QWidget*parent) : IdTableT<Class>(
    []() -> std::vector<Class> {
        return cdb::all_classes(Sorted::YES);
    }, parent
)
{
    makeColumn<std::string>("Key", [](Class c) -> std::string {
        return cdb::key(c);
    });
    makeColumn<std::string> ("Name", [](Class c) -> std::string {
        return cdb::name(c);
    });
    setWindowTitle(tr("Classes"));
}

ClassTable::~ClassTable()
{
}

#include "moc_ClassTable.cpp"
