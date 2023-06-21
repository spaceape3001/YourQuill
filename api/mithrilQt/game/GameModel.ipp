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
    std::optional<IdColumn>     GameModel::resolve(Column col, ColOpts opts)
    {
        switch(col){
        case Column::Id:
            return column::game_id(opts);
        case Column::Key:
            return column::game_key(opts);
        case Column::Title:
            return column::game_title(opts);
        default:
            return {};
        }
    }

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
    
    void    GameModel::addColumn(Column col, ColOpts opts)
    {
        auto    cc  = resolve(col, opts);
        if(cc)
            addColumn(std::move(*cc));
    }
    
    void    GameModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }

    void    GameModel::setColumn(Column col, ColOpts opts)
    {
        auto    cc = resolve(col, opts);
        if(cc)
            setColumn(std::move(*cc));
    }
}
