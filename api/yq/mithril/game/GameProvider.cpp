////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/game/GameProvider.hpp>
#include <yq/mithril/game/GameCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_games()
    {
        return [](){
            return ids(cdb::all_games(Sorted::YES));
        };
    }
}

