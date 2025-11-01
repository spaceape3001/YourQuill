////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/place/Place.hpp>
#include <yq/mithril/place/PlaceCDB.hpp>
#include <yq/mithril/place/PlaceSearch.hpp>
#include <yq/mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    PlaceVector  search(WebContext&ctx, place_k)
    {
        WebSearcher<Place>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_places(Sorted::YES);
        return ws.data;
    }
}
