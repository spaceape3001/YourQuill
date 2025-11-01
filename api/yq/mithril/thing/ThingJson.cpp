////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/thing/Thing.hpp>
#include <yq/mithril/thing/ThingCDB.hpp>
#include <yq/mithril/thing/ThingJson.hpp>
#include <yq/net/json.hpp>

namespace yq::mithril {
    json json_(Thing x)
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
    
    json json_(const ThingVector& xs)
    {
        json j = json::array();
        for(Thing x : xs)
            j.push_back(json_(x));
        return j;
    }
}
