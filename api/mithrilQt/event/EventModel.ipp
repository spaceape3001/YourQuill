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
    std::optional<IdColumn>     EventModel::resolve(Column col, ColOpts opts)
    {
        switch(col){
        case Column::Id:
            return column::event_id(opts);
        case Column::Key:
            return column::event_key(opts);
        case Column::Title:
            return column::event_title(opts);
        default:
            return {};
        }
    }

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
        auto    cc  = resolve(col, opts);
        if(cc)
            addColumn(std::move(*cc));
    }

    void    EventModel::addColumns(std::span<const Column> columns)
    {
        for(Column c : columns)
            addColumn(c);
    }

    void    EventModel::setColumn(Column col, ColOpts opts)
    {
        auto    cc = resolve(col, opts);
        if(cc)
            setColumn(std::move(*cc));
    }
    
}
