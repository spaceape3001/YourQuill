////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/novel/Novel.hpp>
#include <yq/mithril/novel/NovelCDB.hpp>
#include <yq/mithril/novel/NovelSearch.hpp>
#include <yq/mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    NovelVector  search(WebContext&ctx, novel_k)
    {
        WebSearcher<Novel>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_novels(Sorted::YES);
        return ws.data;
    }
}
