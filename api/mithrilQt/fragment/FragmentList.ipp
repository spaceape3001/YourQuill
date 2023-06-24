////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FragmentList.hpp"

namespace yq::mithril {
    FragmentList::FragmentList(all_t, QWidget*parent) : FragmentList(ALL, defColumn, parent)
    {
    }
    
    FragmentList::FragmentList(all_t, Column col, QWidget*parent) : 
        IdListT<Fragment>(ALL, parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Fragments");
    }

    FragmentList::~FragmentList()
    {
    }
}

