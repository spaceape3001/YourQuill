////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/user/User.hpp>
#include <mithril/user/UserCDB.hpp>
#include <mithril/user/UserSearch.hpp>
#include <mithril/web/WebSearcher.hpp>

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
