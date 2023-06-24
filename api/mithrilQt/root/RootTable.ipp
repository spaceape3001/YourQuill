////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RootTable.hpp"

namespace yq::mithril {
    std::span<const ColumnSpec>  RootTable::defColumns() 
    {
        static ColumnSpec   s_data[] = { { Column::Key, ColOpt::Icon}, Column::Path };
        return std::span<const ColumnSpec>(std::begin(s_data), std::end(s_data));
    }


    RootTable::RootTable(all_t, QWidget*parent) : RootTable(ALL, defColumns(), parent)
    {
    }
    
    RootTable::RootTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent) : 
        RootTable(ALL, span(columns), parent)
    {
    }
    
    RootTable::RootTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent) : 
        IdTableT<Root>(ALL, parent)
    {
        model()->addColumns(columns);
        model()->reload();
        setWindowTitle("Roots");
    }

    RootTable::~RootTable()
    {
    }
}

