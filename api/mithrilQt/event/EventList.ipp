////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "EventList.hpp"

namespace yq::mithril {
    EventList::EventList(all_t, QWidget*parent) : EventList(ALL, defColumn, parent)
    {
    }
    
    EventList::EventList(all_t, Column col, QWidget*parent) : 
        IdListT<Event>(ALL, parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Events");
    }

    EventList::~EventList()
    {
    }
}

