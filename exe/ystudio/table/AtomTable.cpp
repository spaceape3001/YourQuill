////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/atom.hpp>
#include "AtomTable.hpp"


AtomTable::AtomTable(QWidget*parent) : IdTableT<Atom>(
    []() -> std::vector<Atom> {
        return cdb::all_atoms(Sorted::YES);
    }, parent
)
{
    makeColumn<uint64_t> ("ID"sv, [](Atom a) -> uint64_t { return a.id; });
    makeColumn<std::string> ("Name", [](Atom a) -> std::string {
        return cdb::name(a);
    });
}

AtomTable::~AtomTable()
{
}

#include "moc_AtomTable.cpp"
