////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "BookTable.hpp"

namespace yq::mithril {
    std::span<const ColumnSpec>  BookTable::defColumns() 
    {
        static ColumnSpec   s_data[] = { { Column::Key, ColOpt::Icon}, Column::Title };
        return std::span<const ColumnSpec>(std::begin(s_data), std::end(s_data));
    }


    BookTable::BookTable(all_t, QWidget*parent) : BookTable(ALL, defColumns(), parent)
    {
    }
    
    BookTable::BookTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent) : 
        BookTable(ALL, span(columns), parent)
    {
    }
    
    BookTable::BookTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent) : 
        IdTableT<Book>(ALL, parent)
    {
        model()->addColumns(columns);
        model()->reload();
        setWindowTitle("Books");
    }

    BookTable::~BookTable()
    {
    }
}

