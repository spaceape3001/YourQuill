////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/thing/Thing.hpp>
#include <yq/mithril/thing/ThingCDB.hpp>
#include <yq/mithril/thing/ThingSearch.hpp>
#include <yq/mithril/web/WebSearcher.hpp>

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
