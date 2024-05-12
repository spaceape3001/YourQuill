////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/category/Category.hpp>
#include <mithril/category/CategoryCDB.hpp>
#include <mithril/category/CategorySearch.hpp>

namespace yq::mithril {
    CategoryVector  search(WebContext&ctx, category_t)
    {
        WebSearcher<Category>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_categories(Sorted::YES);
        return ws.data;
    }
}
