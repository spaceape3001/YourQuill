////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/tag/Tag.hpp>
#include <mithril/tag/TagCDB.hpp>
#include <mithril/tag/TagSearch.hpp>
#include <mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    TagVector  search(WebContext&ctx, tag_k)
    {
        WebSearcher<Tag>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_tags(Sorted::YES);
        return ws.data;
    }
}
