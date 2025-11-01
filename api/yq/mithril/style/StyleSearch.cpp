////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/style/Style.hpp>
#include <yq/mithril/style/StyleCDB.hpp>
#include <yq/mithril/style/StyleSearch.hpp>
#include <yq/mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    StyleVector  search(WebContext&ctx, style_k)
    {
        WebSearcher<Style>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_styles(Sorted::YES);
        return ws.data;
    }
}
