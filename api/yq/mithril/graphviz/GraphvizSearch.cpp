////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/graphviz/Graphviz.hpp>
#include <yq/mithril/graphviz/GraphvizCDB.hpp>
#include <yq/mithril/graphviz/GraphvizSearch.hpp>
#include <yq/mithril/web/WebSearcher.hpp>

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
