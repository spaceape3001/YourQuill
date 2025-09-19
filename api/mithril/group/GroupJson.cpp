////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/group/Group.hpp>
#include <mithril/group/GroupCDB.hpp>
#include <mithril/group/GroupJson.hpp>
#include <yq/net/json.hpp>

namespace yq::mithril {
    json json_(Group x)
    {
        json j{
            { "id", x.id },
            { "key", cdb::key(x) },
            { "name", cdb::name(x) }
        };
        return j;
    }
    
    json json_(const GroupVector& xs)
    {
        json j = json::array();
        for(Group x : xs)
            j.push_back(json_(x));
        return j;
    }
}
