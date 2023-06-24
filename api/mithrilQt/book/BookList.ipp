////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "BookList.hpp"

namespace yq::mithril {
    BookList::BookList(all_t, QWidget*parent) : BookList(ALL, defColumn, parent)
    {
    }
    
    BookList::BookList(all_t, Column col, QWidget*parent) : 
        IdListT<Book>(ALL, parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Books");
    }

    BookList::~BookList()
    {
    }
}

