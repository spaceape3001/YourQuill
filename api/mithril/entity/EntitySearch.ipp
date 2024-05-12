////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/entity/Entity.hpp>
#include <mithril/entity/EntityCDB.hpp>
#include <mithril/entity/EntitySearch.hpp>

namespace yq::mithril {
    EntityVector  search(WebContext&ctx, entity_t)
    {
        WebSearcher<Entity>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_entities(Sorted::YES);
        return ws.data;
    }
}
