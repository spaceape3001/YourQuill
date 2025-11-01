////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/group/Group.hpp>
#include <yq/mithril/group/GroupCDB.hpp>
#include <yq/mithril/group/GroupSearch.hpp>
#include <yq/mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    GroupVector  search(WebContext&ctx, group_k)
    {
        WebSearcher<Group>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_groups(Sorted::YES);
        return ws.data;
    }
}
