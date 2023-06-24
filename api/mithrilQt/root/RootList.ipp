////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RootList.hpp"

namespace yq::mithril {
    RootList::RootList(all_t, QWidget*parent) : RootList(ALL, defColumn, parent)
    {
    }
    
    RootList::RootList(all_t, Column col, QWidget*parent) : 
        IdListT<Root>(ALL, parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Roots");
    }

    RootList::~RootList()
    {
    }
}

