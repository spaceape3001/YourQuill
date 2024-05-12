////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/event/Event.hpp>
#include <mithril/event/EventCDB.hpp>
#include <mithril/event/EventJson.hpp>

namespace yq::mithril {
    json json_(Event x)
    {
        json j{
            { "id", x.id },
            { "key", cdb::key(x) },
            { "title", cdb::title(x) }
        };
        return j;
    }
    
    json json_(const EventVector& xs)
    {
        json j = json::array();
        for(Event x : xs)
            j.push_back(json_(x));
        return j;
    }
}
