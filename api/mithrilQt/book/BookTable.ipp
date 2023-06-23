////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "BookTable.hpp"
#include "BookModel.hpp"

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
        IdTableT<Book>(new BookModel(IdModel::Type::Table, ALL), parent)
    {
        model()->addColumns(columns);
        model()->reload();
        setWindowTitle("Books");
    }

    BookTable::~BookTable()
    {
    }
    
    BookModel*          BookTable::model()
    {
        return static_cast<BookModel*>(m_model);
    }
    
    const BookModel*    BookTable::model() const
    {
        return static_cast<const BookModel*>(m_model);
    }
}

