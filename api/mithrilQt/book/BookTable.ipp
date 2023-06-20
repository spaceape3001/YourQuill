////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "BookTable.hpp"
#include "BookModel.hpp"

namespace yq::mithril {
    std::span<const Column>  BookTable::defColumns() 
    {
        static Column   s_data[] = { Column::Key, Column::Title };
        return std::span<const Column>(std::begin(s_data), std::end(s_data));
    }


    BookTable::BookTable(all_t, QWidget*parent) : BookTable(ALL, defColumns(), parent)
    {
    }
    
    BookTable::BookTable(all_t, std::initializer_list<Column> columns, QWidget*parent) : 
        BookTable(ALL, span(columns), parent)
    {
    }
    
    BookTable::BookTable(all_t, std::span<const Column> columns, QWidget*parent) : 
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

