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
    
        static std::optional<IdColumn>     resolve(Column, ColOpts opts=ColOpts());
    
        GameModel(Type t, QObject* parent=nullptr) : GameModel(t, ALL, parent) {}
        GameModel(Type t, all_t, QObject* parent=nullptr);
        ~GameModel();
        
        using IdModelT<Game>::addColumn;
        void    addColumn(Column, ColOpts opts=ColOpts());
        void    addColumns(std::span<const Column> columns);
        
        using IdModelT<Game>::setColumn;
        void    setColumn(Column, ColOpts opts=ColOpts());

    private:
        GameModel(Type t, Game, IdProvider&&, QObject*parent);
    };
}
