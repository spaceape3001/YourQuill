////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "GameArg.hpp"
#include <mithril/web/WebContext.hpp>
#include <mithril/game/GameCDB.hpp>
#include <yq/text/parse.hpp>
#include <yq/text/transform.hpp>

namespace yq::mithril::arg {
    Game game(std::string_view  arg_string)
    {
        arg_string   = trimmed(arg_string);
        if(arg_string.empty())
            return Game{};
            
        Game t   = cdb::game( arg_string);
        if(t)
            return t;
        return game_id(arg_string);
    }
    
    Game game(const WebContext&ctx, bool *detected)
    {
        std::string    k    = ctx.find_query("id");
        if(!k.empty())
            return game_id(k);
        
        k       = ctx.find_query("key");
        if(!k.empty())
            return game_key(k);
        
        k       = ctx.find_query("game");
        if(!k.empty())
            return game(k);
        return Game{};
    }
    
    
    Game game(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return game(arg_string);
    }
    
    Game game(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return game(arg_string);
    }

    Game game_id(std::string_view arg_string)
    {
        auto vv = to_uint64(arg_string);
        if(!vv)
            return Game{};
        if(!cdb::exists_game(*vv))
            return Game{};
        return Game{*vv};
    }

    Game game_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return game_id(arg_string);
    }
    
    Game game_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return game_id(arg_string);
    }
    
    Game game_key(std::string_view arg_string)
    {
        return cdb::game(trimmed(arg_string));
    }
    
    Game game_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return game_key(arg_string);
    }
    
    Game game_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return game_key(arg_string);
    }
}
