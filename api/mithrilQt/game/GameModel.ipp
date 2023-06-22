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
}
