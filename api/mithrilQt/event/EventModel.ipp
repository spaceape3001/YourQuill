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
    EventModel::EventModel(Type t, all_t, QObject* parent) : 
        EventModel(t, Event(), provider::all_events(), parent)
    {
    }

    EventModel::EventModel(Type t, Event rt, IdProvider&& prov, QObject*parent) : 
        IdModelT<Event>(t, rt, std::move(prov), parent)
    {
    }
    
    EventModel::~EventModel()
    {
    }
    
    void    EventModel::addColumn(Column col, ColOpts opts)
    {
        switch(col){
        case Column::Id:
            addColumn(column::event_id(opts));
            break;
        case Column::Key:
            addColumn(column::event_key(opts));
            break;
        case Column::Title:
            addColumn(column::event_title(opts));
            break;
        default:
            break;
        }
    }

    void    EventModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }
    
}
