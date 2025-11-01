////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/value/Value.hpp>
#include <yq/mithril/value/ValueCDB.hpp>
#include <yq/mithril/value/ValueSearch.hpp>
#include <yq/mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    ValueVector  search(WebContext&ctx, value_k)
    {
        WebSearcher<Value>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_values(Sorted::YES);
        return ws.data;
    }
}
