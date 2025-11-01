////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "GamePost.hpp"

#include <yq/mithril/game/GameArg.hpp>
#include <yq/mithril/web/WebContext.hpp>
#include <yq/mithril/game/Game.hpp>
#include <yq/text/copy.hpp>

namespace yq::mithril::post {
    Game game(WebContext&ctx, bool *detected)
    {
        ctx.decode_post();
        
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.rx_post.first("game");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return arg::game_id(k);
        }
        return Game();
    }
    
    Game game(WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_name));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::game_id(arg_string);
    }
    
    Game game(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_names));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::game_id(arg_string);
    }
}
