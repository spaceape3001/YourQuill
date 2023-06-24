////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FieldTable.hpp"

namespace yq::mithril {
    std::span<const ColumnSpec>  FieldTable::defColumns() 
    {
        static ColumnSpec   s_data[] = {{ Column::Key, ColOpt::Icon}, Column::Name };
        return std::span<const ColumnSpec>(std::begin(s_data), std::end(s_data));
    }


    FieldTable::FieldTable(all_t, QWidget*parent) : FieldTable(ALL, defColumns(), parent)
    {
    }
    
    FieldTable::FieldTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent) : 
        FieldTable(ALL, span(columns), parent)
    {
    }
    
    FieldTable::FieldTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent) : 
        IdTableT<Field>(ALL, parent)
    {
        model()->addColumns(columns);
        model()->reload();
        setWindowTitle("Fields");
    }

    FieldTable::~FieldTable()
    {
    }
}


