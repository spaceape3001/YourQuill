////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/style/Style.hpp>
#include <mithril/style/StyleCDB.hpp>
#include <mithril/style/StyleSearch.hpp>

namespace yq::mithril {
    StyleVector  search(WebContext&ctx, style_t)
    {
        WebSearcher<Style>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_styles(Sorted::YES);
        return ws.data;
    }
}
