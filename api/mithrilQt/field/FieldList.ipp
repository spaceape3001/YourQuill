////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FieldList.hpp"

namespace yq::mithril {
    FieldList::FieldList(all_t, QWidget*parent) : FieldList(ALL, defColumn, parent)
    {
    }
    
    FieldList::FieldList(all_t, Column col, QWidget*parent) : 
        IdListT<Field>(ALL, parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Fields");
    }

    FieldList::~FieldList()
    {
    }
}

