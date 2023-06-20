////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "EventTable.hpp"
#include "EventModel.hpp"


namespace yq::mithril {
    std::span<const Column>  EventTable::defColumns() 
    {
        static Column   s_data[] = { Column::Key, Column::Title };
        return std::span<const Column>(std::begin(s_data), std::end(s_data));
    }

    EventTable::EventTable(all_t, QWidget*parent) : EventTable(ALL, defColumns(), parent)
    {
    }
    
    EventTable::EventTable(all_t, std::initializer_list<Column> columns, QWidget*parent) : 
        EventTable(ALL, span(columns), parent)
    {
    }
    
    EventTable::EventTable(all_t, std::span<const Column> columns, QWidget*parent) : 
        IdTableT<Event>(new EventModel(IdModel::Type::Table, ALL), parent)
    {
        model()->addColumns(columns);
        model()->reload();
        setWindowTitle("Events");
    }

    EventTable::~EventTable()
    {
    }
    
    
    EventModel*          EventTable::model()
    {
        return static_cast<EventModel*>(m_model);
    }
    
    const EventModel*    EventTable::model() const
    {
        return static_cast<const EventModel*>(m_model);
    }
}

