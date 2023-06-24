////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PlaceList.hpp"

namespace yq::mithril {
    PlaceList::PlaceList(all_t, QWidget*parent) : PlaceList(ALL, defColumn, parent)
    {
    }
    
    PlaceList::PlaceList(all_t, Column col, QWidget*parent) : 
        IdListT<Place>(ALL, parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Places");
    }

    PlaceList::~PlaceList()
    {
    }
}

