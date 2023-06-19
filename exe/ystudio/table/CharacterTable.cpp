////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/character.hpp>
#include "CharacterTable.hpp"


CharacterTable::CharacterTable(QWidget*parent) : IdTableT<Character>(
    []() -> std::vector<Character> {
        return cdb::all_characters(Sorted::YES);
    }, parent
)
{
    makeColumn<std::string>("Key", [](Character ch) -> std::string {
        return cdb::key(ch);
    });
    makeColumn<std::string> ("Name", [](Character ch) -> std::string {
        return cdb::name(ch);
    });
    setWindowTitle(tr("Characters"));
}

CharacterTable::~CharacterTable()
{
}

#include "moc_CharacterTable.cpp"
