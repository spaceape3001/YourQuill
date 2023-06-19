////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>
#include <mithril/game/Game.hpp>

namespace yq::mithril {        
    namespace cdb {
        std::vector<Game>   all_games(Sorted sorted=Sorted());
        Atom                atom(Game);
        Game                game(Atom);
        Class               game_class();
        bool                exists(Game);
        bool                exists_game(uint64_t);
        std::string         key(Game);
        std::string         title(Game);
    }
}
