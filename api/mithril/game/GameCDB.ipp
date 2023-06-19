////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/AtomCDB.hpp>
#include <mithril/class/ClassCDB.hpp>
#include <mithril/game/GameCDB.hpp>

namespace yq::mithril::cdb {
    std::vector<Game>      all_games(Sorted sorted)
    {
        return blind_remap(all_atoms(game_class(), sorted), Game());
    }
    
    Atom  atom(Game ch)
    {
        return Atom{ ch.id };
    }

    Game  game(Atom at)
    {
        return is(at, game_class()) ? Game(at.id) : Game();
    }

    Class  game_class()
    {
        static const Class  cls = db_class("Game");
        return cls;
    }

    bool  exists(Game ch)
    {
        return is(Atom(ch.id), game_class());
    }

    std::string         key(Game g)
    {
        return key(atom(g));
    }
    
    std::string         title(Game g)
    {
        return name(atom(g));
    }
}
