////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/tag/Tag.hpp>
#include <yq/mithril/tag/TagCDB.hpp>
#include <yq/mithril/tag/TagSearch.hpp>
#include <yq/mithril/web/WebSearcher.hpp>

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
