////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AtomList.hpp"

namespace yq::mithril {
    AtomList::AtomList(all_t, QWidget*parent) : AtomList(ALL, defColumn, parent)
    {
    }
    
    AtomList::AtomList(all_t, Column col, QWidget*parent) : 
        IdListT<Atom>(ALL, parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Atoms");
    }

    AtomList::~AtomList()
    {
    }
}

