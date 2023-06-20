////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/game/GameProvider.hpp>
#include <mithril/game/GameCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_games()
    {
        return [](){
            return ids(cdb::all_games(Sorted::YES));
        };
    }
}

