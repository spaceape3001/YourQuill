////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/user/User.hpp>
#include <mithril/user/UserCDB.hpp>
#include <mithril/user/UserSearch.hpp>

namespace yq::mithril {
    UserVector  search(WebContext&ctx, user_t)
    {
        WebSearcher<User>   ws(ctx);
        //search(ws, FOLDER);
        //search(ws, TAG);
        if(ws.first && ws.data.empty())
            ws.data  = cdb::all_users(Sorted::YES);
        return ws.data;
    }
}
