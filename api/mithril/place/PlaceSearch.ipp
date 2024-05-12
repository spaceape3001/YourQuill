////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/place/Place.hpp>
#include <mithril/place/PlaceCDB.hpp>
#include <mithril/place/PlaceSearch.hpp>

namespace yq::mithril {
    PlaceVector  search(WebContext&ctx, place_t)
    {
        WebSearcher<Place>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_places(Sorted::YES);
        return ws.data;
    }
}
