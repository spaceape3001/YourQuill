////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/entity/Entity.hpp>
#include <mithril/entity/EntityCDB.hpp>
#include <mithril/entity/EntityJson.hpp>
#include <nlohmann/json.hpp>

namespace yq::mithril {
    json json_(Entity x)
    {
        json j{
            { "id", x.id },
            { "key", cdb::key(x) },
            { "name", cdb::name(x) }
        };

        return j;
    }
    
    json json_(const EntityVector& xs)
    {
        json j = json::array();
        for(Entity x : xs)
            j.push_back(json_(x));
        return j;
    }
}
