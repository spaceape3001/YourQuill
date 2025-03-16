////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/entity/Entity.hpp>
#include <mithril/entity/EntityCDB.hpp>
#include <mithril/entity/EntitySearch.hpp>
#include <mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    EntityVector  search(WebContext&ctx, entity_k)
    {
        WebSearcher<Entity>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_entities(Sorted::YES);
        return ws.data;
    }
}
