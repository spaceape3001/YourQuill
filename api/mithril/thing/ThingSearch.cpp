////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/thing/Thing.hpp>
#include <mithril/thing/ThingCDB.hpp>
#include <mithril/thing/ThingSearch.hpp>
#include <mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    ThingVector  search(WebContext&ctx, thing_k)
    {
        WebSearcher<Thing>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_things(Sorted::YES);
        return ws.data;
    }
}
