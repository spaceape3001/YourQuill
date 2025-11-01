////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/place/Place.hpp>
#include <mithril/place/PlaceCDB.hpp>
#include <mithril/place/PlaceSearch.hpp>
#include <mithril/web/WebSearcher.hpp>

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
