////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/user/User.hpp>
#include <yq/mithril/user/UserCDB.hpp>
#include <yq/mithril/user/UserSearch.hpp>
#include <yq/mithril/web/WebSearcher.hpp>

namespace yq::mithril {
    UserVector  search(WebContext&ctx, user_k)
    {
        WebSearcher<User>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_users(Sorted::YES);
        return ws.data;
    }
}
