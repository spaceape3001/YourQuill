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
    
        static std::span<const Column>  defColumns();
    
        GameModel(Type t) : GameModel(t, ALL) {}
        GameModel(Type t, all_t, std::span<const Column> cols=defColumns(), QObject* parent=nullptr);
        ~GameModel();
        
        using IdModelT<Game>::addColumn;
        void    addColumn(Column);
        
    private:
        GameModel(Type t, Game, IdProvider&&, std::span<const Column> cols, QObject*parent);
    };
}
