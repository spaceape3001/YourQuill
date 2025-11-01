////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/game/Game.hpp>
#include <yq/mithril/game/GameCDB.hpp>
#include <yq/mithril/game/GameSearch.hpp>
#include <yq/mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    GameVector  search(WebContext&ctx, game_k)
    {
        WebSearcher<Game>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_games(Sorted::YES);
        return ws.data;
    }
}
