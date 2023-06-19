////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/game.hpp>
#include <mithril/atom/AtomCDB.hpp>
#include "GameTable.hpp"


GameTable::GameTable(QWidget*parent) : IdTableT<Game>(
    []() -> std::vector<Game> {
        return cdb::all_games(Sorted::YES);
    }, parent
)
{
    makeColumn<uint64_t> ("ID"sv, [](Game b) -> uint64_t { return b.id; });
    makeColumn<std::string> ("Name", [](Game b) -> std::string {
        return cdb::name(cdb::atom(b));
    });
}

GameTable::~GameTable()
{
}

#include "moc_GameTable.cpp"
