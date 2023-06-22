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
}
