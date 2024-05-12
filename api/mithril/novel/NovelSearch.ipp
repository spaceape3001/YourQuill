////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/novel/Novel.hpp>
#include <mithril/novel/NovelCDB.hpp>
#include <mithril/novel/NovelSearch.hpp>

namespace yq::mithril {
    NovelVector  search(WebContext&ctx, novel_t)
    {
        WebSearcher<Novel>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_novels(Sorted::YES);
        return ws.data;
    }
}
