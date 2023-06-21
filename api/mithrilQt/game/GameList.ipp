////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "GameList.hpp"
#include "GameModel.hpp"

namespace yq::mithril {
    GameList::GameList(all_t, QWidget*parent) : GameList(ALL, defColumn, parent)
    {
    }
    
    GameList::GameList(all_t, Column col, QWidget*parent) : 
        IdListT<Game>(new GameModel(IdModel::Type::List, ALL), parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Games");
    }

    GameList::~GameList()
    {
    }
    
    GameModel*          GameList::model()
    {
        return static_cast<GameModel*>(m_model);
    }
    
    const GameModel*    GameList::model() const
    {
        return static_cast<const GameModel*>(m_model);
    }
}

