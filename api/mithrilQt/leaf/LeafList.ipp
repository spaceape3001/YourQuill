////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "LeafList.hpp"

namespace yq::mithril {
    LeafList::LeafList(all_t, QWidget*parent) : LeafList(ALL, defColumn, parent)
    {
    }
    
    LeafList::LeafList(all_t, Column col, QWidget*parent) : 
        IdListT<Leaf>(ALL, parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Leafs");
    }

    LeafList::~LeafList()
    {
    }
}

