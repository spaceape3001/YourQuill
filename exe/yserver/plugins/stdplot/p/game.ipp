////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/game/GameJson.hpp>
#include <yq/mithril/game/GameSearch.hpp>
#include <yq/mithril/wksp/Workspace.hpp>

namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

        json p_api_game(WebContext& ctx)
        {
            Game    v   = arg::game(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            return json_(v);
        }
        
        json p_api_game_key(WebContext& ctx)
        {
            Game    v   = arg::game(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            json j{
                { "key", cdb::key(v) }
            };
            return j;
        }
        
        json p_api_games(WebContext& ctx)
        {
            GameVector  ret = search(ctx, GAME);
            return json{
                { "games", json_(ret) }
            };
        }
        
    //  -----------------------------------------------------------------------
    //      PAGES
    //  -----------------------------------------------------------------------
    
    //  -----------------------------------------------------------------------
    //      ADMIN
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      DEV
    //  -----------------------------------------------------------------------

        void    p_dev_games(WebHtml& h)
        {
            h.title() << "All Games";
            dev_table(h, all_games(Sorted::YES));
        }
        

    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_game_pages()
        {
            reg_webpage<p_api_game>("/api/game").argument("ID", "Game ID");
            reg_webpage<p_api_game_key>("/api/game/key").argument("ID", "Game ID");
            reg_webpage<p_api_games>("/api/games");
            
            reg_webpage<p_dev_games>("/dev/games");
            reg_webtemplate("/games", wksp::shared("www/games.ht"sv));
        }
}
