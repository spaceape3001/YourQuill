////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/group/Group.hpp>
#include <yq/mithril/group/GroupCDB.hpp>
#include <yq/mithril/group/GroupJson.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/net/json.hpp>

namespace yq::mithril {
    json json_(Group x)
    {
        json j{
            { "brief", cdb::brief(x) },
            { "icon", cdb::icon(x).id },
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
