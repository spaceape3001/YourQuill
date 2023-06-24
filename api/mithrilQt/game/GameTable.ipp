////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "GameTable.hpp"

namespace yq::mithril {
    std::span<const ColumnSpec>  GameTable::defColumns() 
    {
        static ColumnSpec   s_data[] = { { Column::Key, ColOpt::Icon}, Column::Title };
        return std::span<const ColumnSpec>(std::begin(s_data), std::end(s_data));
    }


    GameTable::GameTable(all_t, QWidget*parent) : GameTable(ALL, defColumns(), parent)
    {
    }
    
    GameTable::GameTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent) : 
        GameTable(ALL, span(columns), parent)
    {
    }
    
    GameTable::GameTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent) : 
        IdTableT<Game>(ALL, parent)
    {
        model()->addColumns(columns);
        model()->reload();
        setWindowTitle("Games");
    }

    GameTable::~GameTable()
    {
    }
}

