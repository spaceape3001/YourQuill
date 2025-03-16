////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/character/Character.hpp>
#include <mithril/character/CharacterCDB.hpp>
#include <mithril/character/CharacterJson.hpp>
#include <nlohmann/json.hpp>

namespace yq::mithril {
    json json_(Character x)
    {
        json j{
            { "id", x.id },
            { "key", cdb::key(x) },
            { "name", cdb::name(x) }
        };
        return j;
    }
    
    json json_(const CharacterVector& xs)
    {
        json j = json::array();
        for(Character x : xs)
            j.push_back(json_(x));
        return j;
    }
}
