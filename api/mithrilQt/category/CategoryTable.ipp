////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CategoryTable.hpp"

namespace yq::mithril {
    std::span<const ColumnSpec>  CategoryTable::defColumns() 
    {
        static ColumnSpec   s_data[] = { { Column::Key, ColOpt::Icon}, Column::Name };
        return std::span<const ColumnSpec>(std::begin(s_data), std::end(s_data));
    }

    CategoryTable::CategoryTable(all_t, QWidget*parent) : CategoryTable(ALL, defColumns(), parent)
    {
    }
    
    CategoryTable::CategoryTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent) : 
        CategoryTable(ALL, span(columns), parent)
    {
    }
    
    CategoryTable::CategoryTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent) : 
        IdTableT<Category>(ALL, parent)
    {
        model()->addColumns(columns);
        model()->reload();
        setWindowTitle("Categories");
    }

    CategoryTable::~CategoryTable()
    {
    }
}

