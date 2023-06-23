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
        static std::span<const ColumnSpec>  defColumns();

        GameTable(all_t, QWidget*parent=nullptr);
        GameTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent=nullptr);
        GameTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent=nullptr);
        ~GameTable();
        
        GameModel*          model();
        const GameModel*    model() const;
    };
}
