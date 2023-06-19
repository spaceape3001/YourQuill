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
    makeColumn<std::string>("Key", [](Atom a) -> std::string {
        return cdb::key(a);
    });
    makeColumn<std::string> ("Name", [](Atom a) -> std::string {
        return cdb::name(a);
    });
    
    setWindowTitle(tr("Atoms"));
}

AtomTable::~AtomTable()
{
}

#include "moc_AtomTable.cpp"
