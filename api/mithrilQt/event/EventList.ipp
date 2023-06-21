////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "EventList.hpp"
#include "EventModel.hpp"

namespace yq::mithril {
    EventList::EventList(all_t, QWidget*parent) : EventList(ALL, defColumn, parent)
    {
    }
    
    EventList::EventList(all_t, Column col, QWidget*parent) : 
        IdListT<Event>(new EventModel(IdModel::Type::List, ALL), parent)
    {
        model()->setColumn(col);
        model()->reload();
        setWindowTitle("Events");
    }

    EventList::~EventList()
    {
    }
    
    EventModel*          EventList::model()
    {
        return static_cast<EventModel*>(m_model);
    }
    
    const EventModel*    EventList::model() const
    {
        return static_cast<const EventModel*>(m_model);
    }
}

