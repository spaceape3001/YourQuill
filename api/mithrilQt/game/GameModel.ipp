////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "GameModel.hpp"
#include "GameColumn.hpp"
#include <mithril/game/GameProvider.hpp>

namespace yq::mithril {
    std::span<const Column>  GameModel::defColumns() 
    {
        static Column   s_data[] = { Column::Key, Column::Title };
        return std::span<const Column>(std::begin(s_data), std::end(s_data));
    }

    GameModel::GameModel(Type t, all_t, std::span<const Column> cols, QObject* parent) : 
        GameModel(t, Game(), provider::all_games(), cols, parent)
    {
    }

    GameModel::GameModel(Type t, Game rt, IdProvider&& prov, std::span<const Column> cols, QObject*parent) : 
        IdModelT<Game>(t, rt, std::move(prov), parent)
    {
        for(Column c : cols)
            addColumn(c);
        reload();
    }
    
    GameModel::~GameModel()
    {
    }
    
    void    GameModel::addColumn(Column col)
    {
        switch(col){
        case Column::Id:
            addColumn(column::game_id());
            break;
        case Column::Key:
            addColumn(column::game_key());
            break;
        case Column::Title:
            addColumn(column::game_title());
            break;
        default:
            break;
        }
    }
    
}
