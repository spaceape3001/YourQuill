////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CategoryList.hpp"

namespace yq::mithril {
    CategoryList::CategoryList(all_t, QWidget*parent) : CategoryList(ALL, defColumn, parent)
    {
    }
    
    CategoryList::CategoryList(all_t, Column col, QWidget*parent) : 
        IdListT<Category>(ALL, parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Categories");
    }

    CategoryList::~CategoryList()
    {
    }
}

