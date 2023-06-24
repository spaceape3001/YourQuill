////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ClassList.hpp"

namespace yq::mithril {
    ClassList::ClassList(all_t, QWidget*parent) : ClassList(ALL, defColumn, parent)
    {
    }
    
    ClassList::ClassList(all_t, Column col, QWidget*parent) : 
        IdListT<Class>(ALL, parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Classes");
    }

    ClassList::~ClassList()
    {
    }
}

