////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/game/Game.hpp>
#include <mithril/game/GameCDB.hpp>
#include <mithril/game/GameSearch.hpp>
#include <mithril/web/WebSearcher.hpp>

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
