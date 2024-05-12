////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/group/Group.hpp>
#include <mithril/group/GroupCDB.hpp>
#include <mithril/group/GroupSearch.hpp>

namespace yq::mithril {
    GroupVector  search(WebContext&ctx, group_t)
    {
        WebSearcher<Group>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_groups(Sorted::YES);
        return ws.data;
    }
}
