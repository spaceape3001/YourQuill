////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/graphviz/Graphviz.hpp>
#include <mithril/graphviz/GraphvizCDB.hpp>
#include <mithril/graphviz/GraphvizSearch.hpp>
#include <mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    GraphvizVector  search(WebContext&ctx, graphviz_k)
    {
        WebSearcher<Graphviz>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_graphvizs(Sorted::YES);
        return ws.data;
    }
}
