////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "EventModel.hpp"
#include "EventColumn.hpp"
#include <mithril/event/EventProvider.hpp>

namespace yq::mithril {
    std::span<const Column>  EventModel::defColumns() 
    {
        static Column   s_data[] = { Column::Key, Column::Title };
        return std::span<const Column>(std::begin(s_data), std::end(s_data));
    }

    EventModel::EventModel(Type t, all_t, std::span<const Column> cols, QObject* parent) : 
        EventModel(t, Event(), provider::all_events(), cols, parent)
    {
    }

    EventModel::EventModel(Type t, Event rt, IdProvider&& prov, std::span<const Column> cols, QObject*parent) : 
        IdModelT<Event>(t, rt, std::move(prov), parent)
    {
        for(Column c : cols)
            addColumn(c);
        reload();
    }
    
    EventModel::~EventModel()
    {
    }
    
    void    EventModel::addColumn(Column col)
    {
        switch(col){
        case Column::Id:
            addColumn(column::event_id());
            break;
        case Column::Key:
            addColumn(column::event_key());
            break;
        case Column::Title:
            addColumn(column::event_title());
            break;
        default:
            break;
        }
    }
    
}
