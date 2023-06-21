////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "BookList.hpp"
#include "BookModel.hpp"

namespace yq::mithril {
    BookList::BookList(all_t, QWidget*parent) : BookList(ALL, defColumn, parent)
    {
    }
    
    BookList::BookList(all_t, Column col, QWidget*parent) : 
        IdListT<Book>(new BookModel(IdModel::Type::List, ALL), parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Books");
    }

    BookList::~BookList()
    {
    }
    
    BookModel*          BookList::model()
    {
        return static_cast<BookModel*>(m_model);
    }
    
    const BookModel*    BookList::model() const
    {
        return static_cast<const BookModel*>(m_model);
    }
}

