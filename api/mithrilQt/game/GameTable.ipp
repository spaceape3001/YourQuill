////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "GameTable.hpp"
#include "GameModel.hpp"


namespace yq::mithril {
    std::span<const Column>  GameTable::defColumns() 
    {
        static Column   s_data[] = { Column::Key, Column::Title };
        return std::span<const Column>(std::begin(s_data), std::end(s_data));
    }


    GameTable::GameTable(all_t, QWidget*parent) : GameTable(ALL, defColumns(), parent)
    {
    }
    
    GameTable::GameTable(all_t, std::initializer_list<Column> columns, QWidget*parent) : 
        GameTable(ALL, span(columns), parent)
    {
    }
    
    GameTable::GameTable(all_t, std::span<const Column> columns, QWidget*parent) : 
        IdTableT<Game>(new GameModel(IdModel::Type::Table, ALL), parent)
    {
        model()->addColumns(columns);
        model()->reload();
        setWindowTitle("Games");
    }

    GameTable::~GameTable()
    {
    }
    
    
    GameModel*          GameTable::model()
    {
        return static_cast<GameModel*>(m_model);
    }
    
    const GameModel*    GameTable::model() const
    {
        return static_cast<const GameModel*>(m_model);
    }
}

