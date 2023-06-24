////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TagList.hpp"

namespace yq::mithril {
    TagList::TagList(all_t, QWidget*parent) : TagList(ALL, defColumn, parent)
    {
    }
    
    TagList::TagList(all_t, Column col, QWidget*parent) : 
        IdListT<Tag>(ALL, parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Tags");
    }

    TagList::~TagList()
    {
    }
}

