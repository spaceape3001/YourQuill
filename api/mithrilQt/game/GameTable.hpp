////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdTable.hpp>

namespace yq::mithril {
    class GameModel;
    class GameTable : public IdTableT<Game> {
        Q_OBJECT
    public:
        static std::span<const Column>  defColumns();

        GameTable(all_t, QWidget*parent=nullptr);
        GameTable(all_t, std::initializer_list<Column> columns, QWidget*parent=nullptr);
        GameTable(all_t, std::span<const Column> columns, QWidget*parent=nullptr);
        ~GameTable();
        
        GameModel*          model();
        const GameModel*    model() const;
    };
}
