////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/game/Game.hpp>
#include <mithril/game/GameCDB.hpp>
#include <mithril/game/GameJson.hpp>

namespace yq::mithril {
    json json_(Game x)
    {
        json j{
            { "id", x.id },
            { "key", cdb::key(x) },
            { "title", cdb::title(x) }
        };
        return j;
    }
    
    json json_(const GameVector& xs)
    {
        json j = json::array();
        for(Game x : xs)
            j.push_back(json_(x));
        return j;
    }
}
