////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdModel.hpp>
#include <mithril/game/Game.hpp>
#include <mithrilQt/column.hpp>

namespace yq::mithril {
    class GameModel : public IdModelT<Game> {
        Q_OBJECT
    public:
    
    
        GameModel(Type t, QObject* parent=nullptr) : GameModel(t, ALL, parent) {}
        GameModel(Type t, all_t, QObject* parent=nullptr);
        ~GameModel();
        
        using IdModelT<Game>::addColumn;
        void    addColumn(Column);
        void    addColumns(std::span<const Column> columns);
        
    private:
        GameModel(Type t, Game, IdProvider&&, QObject*parent);
    };
}
