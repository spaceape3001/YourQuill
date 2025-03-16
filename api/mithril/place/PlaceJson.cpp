////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/place/Place.hpp>
#include <mithril/place/PlaceCDB.hpp>
#include <mithril/place/PlaceJson.hpp>
#include <nlohmann/json.hpp>

namespace yq::mithril {
    json json_(Place x)
    {
        json j{
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
