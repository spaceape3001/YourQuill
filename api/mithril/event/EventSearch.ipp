////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/event/Event.hpp>
#include <mithril/event/EventCDB.hpp>
#include <mithril/event/EventSearch.hpp>

namespace yq::mithril {
    EventVector  search(WebContext&ctx, event_t)
    {
        WebSearcher<Event>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_events(Sorted::YES);
        return ws.data;
    }
}
