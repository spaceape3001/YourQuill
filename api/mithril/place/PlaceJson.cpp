////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/place/Place.hpp>
#include <mithril/place/PlaceCDB.hpp>
#include <mithril/place/PlaceJson.hpp>
#include <mithril/image/Image.hpp>
#include <yq/net/json.hpp>

namespace yq::mithril {
    json json_(Place x)
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
    
    json json_(const PlaceVector& xs)
    {
        json j = json::array();
        for(Place x : xs)
            j.push_back(json_(x));
        return j;
    }
}
