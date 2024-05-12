////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/graphviz/Graphviz.hpp>
#include <mithril/graphviz/GraphvizCDB.hpp>
#include <mithril/graphviz/GraphvizSearch.hpp>

namespace yq::mithril {
    GraphvizVector  search(WebContext&ctx, graphviz_t)
    {
        WebSearcher<Graphviz>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_graphvizs(Sorted::YES);
        return ws.data;
    }
}
