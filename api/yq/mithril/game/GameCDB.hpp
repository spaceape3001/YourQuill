////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/preamble.hpp>
#include <yq/mithril/game/Game.hpp>

namespace yq::mithril {        
    namespace cdb {
        std::vector<Game>   all_games(Sorted sorted=Sorted());
        Atom                atom(Game);
        std::string         brief(Game);
        Game                game(Atom);
        Game                game(std::string_view);
        Game                game(uint64_t);
        Class               game_class();
        bool                exists(Game);
        bool                exists_game(uint64_t);
        Image               icon(Game);
        std::string         key(Game);
        std::string         name(Game);
        std::string         title(Game);
    }
}
