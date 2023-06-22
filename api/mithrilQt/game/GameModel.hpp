////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdModel.hpp>
#include <mithril/game/Game.hpp>
#include <mithrilQt/preamble.hpp>

namespace yq::mithril {
    class GameModel : public IdModelT<Game> {
        Q_OBJECT
    public:
        GameModel(Type t, QObject* parent=nullptr) : GameModel(t, ALL, parent) {}
        GameModel(Type t, all_t, QObject* parent=nullptr);
        ~GameModel();

    private:
        GameModel(Type t, Game, IdProvider&&, QObject*parent);
    };
}
