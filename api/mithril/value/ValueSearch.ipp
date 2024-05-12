////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/value/Value.hpp>
#include <mithril/value/ValueCDB.hpp>
#include <mithril/value/ValueSearch.hpp>

namespace yq::mithril {
    ValueVector  search(WebContext&ctx, value_t)
    {
        WebSearcher<Value>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_values(Sorted::YES);
        return ws.data;
    }
}
