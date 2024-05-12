////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/thing/Thing.hpp>
#include <mithril/thing/ThingCDB.hpp>
#include <mithril/thing/ThingSearch.hpp>

namespace yq::mithril {
    ThingVector  search(WebContext&ctx, thing_t)
    {
        WebSearcher<Thing>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_things(Sorted::YES);
        return ws.data;
    }
}
