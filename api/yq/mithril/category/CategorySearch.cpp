////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/category/Category.hpp>
#include <yq/mithril/category/CategoryCDB.hpp>
#include <yq/mithril/category/CategorySearch.hpp>
#include <yq/mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    CategoryVector  search(WebContext&ctx, category_k)
    {
        WebSearcher<Category>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_categories(Sorted::YES);
        return ws.data;
    }
}
