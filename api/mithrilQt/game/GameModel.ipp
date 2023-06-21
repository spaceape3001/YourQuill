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
    GameModel::GameModel(Type t, all_t, QObject* parent) : 
        GameModel(t, Game(), provider::all_games(), parent)
    {
    }

    GameModel::GameModel(Type t, Game rt, IdProvider&& prov, QObject*parent) : 
        IdModelT<Game>(t, rt, std::move(prov), parent)
    {
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
    
    void    GameModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }
}