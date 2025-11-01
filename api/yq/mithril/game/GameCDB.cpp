////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

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

    std::string         brief(Game g)
    {
        return brief(atom(g));
    }

    bool  exists(Game ch)
    {
        return is(Atom(ch.id), game_class());
    }

    bool  exists_game(uint64_t id)
    {
        return is(Atom(id), game_class());
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

    Game    game(std::string_view k)
    {
        return game(atom(k));   // TODO -- better lookups
    }
    
    Game    game(uint64_t id)
    {
        return exists_game(id) ? Game(id) : Game();
    }

    Image               icon(Game g)
    {
        return icon(atom(g));
    }

    std::string         key(Game g)
    {
        return key(atom(g));
    }
    
    std::string         name(Game g)
    {
        return name(atom(g));
    }

    std::string         title(Game g)
    {
        return name(atom(g));
    }
}
