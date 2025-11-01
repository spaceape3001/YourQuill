////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/event/Event.hpp>
#include <yq/mithril/event/EventCDB.hpp>
#include <yq/mithril/event/EventSearch.hpp>
#include <yq/mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    EventVector  search(WebContext&ctx, event_k)
    {
        WebSearcher<Event>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_events(Sorted::YES);
        return ws.data;
    }
}
