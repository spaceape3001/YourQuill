////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/fragment.hpp>
#include "FragmentTable.hpp"


FragmentTable::FragmentTable(QWidget*parent) : IdTableT<Fragment>(
    []() -> std::vector<Fragment> {
        return cdb::all_fragments(Sorted::YES);
    }, parent
)
{
    makeColumn<std::string> ("Key", [](Fragment f) -> std::string {
        return cdb::key(f);
    });
    makeColumn<std::string> ("Label", [](Fragment f) -> std::string {
        return cdb::label(f);
    });

    setWindowTitle(tr("Fragments"));
}

FragmentTable::~FragmentTable()
{
}

#include "moc_FragmentTable.cpp"
