////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/character.hpp>
#include <mithril/atom/AtomCDB.hpp>
#include "CharacterTable.hpp"


CharacterTable::CharacterTable(QWidget*parent) : IdTableT<Character>(
    []() -> std::vector<Character> {
        return cdb::all_characters(Sorted::YES);
    }, parent
)
{
    makeColumn<uint64_t> ("ID"sv, [](Character b) -> uint64_t { return b.id; });
    makeColumn<std::string> ("Name", [](Character b) -> std::string {
        return cdb::name(cdb::atom(b));
    });
}

CharacterTable::~CharacterTable()
{
}

#include "moc_CharacterTable.cpp"
