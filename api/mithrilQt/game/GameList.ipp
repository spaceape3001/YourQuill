////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "GameList.hpp"

namespace yq::mithril {
    GameList::GameList(all_t, QWidget*parent) : GameList(ALL, defColumn, parent)
    {
    }
    
    GameList::GameList(all_t, Column col, QWidget*parent) : 
        IdListT<Game>(ALL, parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Games");
    }

    GameList::~GameList()
    {
    }
}

