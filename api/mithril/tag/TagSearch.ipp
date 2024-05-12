////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/tag/Tag.hpp>
#include <mithril/tag/TagCDB.hpp>
#include <mithril/tag/TagSearch.hpp>

namespace yq::mithril {
    TagVector  search(WebContext&ctx, tag_t)
    {
        WebSearcher<Tag>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_tags(Sorted::YES);
        return ws.data;
    }
}
