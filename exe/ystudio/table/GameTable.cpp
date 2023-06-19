////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/game.hpp>
#include "GameTable.hpp"


GameTable::GameTable(QWidget*parent) : IdTableT<Game>(
    []() -> std::vector<Game> {
        return cdb::all_games(Sorted::YES);
    }, parent
)
{
    makeColumn<std::string>("Key", [](Game g) -> std::string {
        return cdb::key(g);
    });
    makeColumn<std::string> ("Title", [](Game g) -> std::string {
        return cdb::title(g);
    });

    setWindowTitle(tr("Games"));
}

GameTable::~GameTable()
{
}

#include "moc_GameTable.cpp"
