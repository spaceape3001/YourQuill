////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/user/User.hpp>
#include <mithril/user/UserCDB.hpp>
#include <mithril/user/UserInfo.hpp>
#include <mithril/user/UserJson.hpp>
#include <yq/net/json.hpp>

namespace yq::mithril {
    json json_(User x)
    {
        User::Info  i   = cdb::info(x);
        json j{
            { "brief", i.brief },
            { "document", i.doc.id },
            { "icon", i.icon.id },
            { "id", x.id },
            { "key", i.key },
            { "name", i.name }
        };
        return j;
    }
    
    json json_(const UserVector& xs)
    {
        json j = json::array();
        for(User x : xs)
            j.push_back(json_(x));
        return j;
    }
}
