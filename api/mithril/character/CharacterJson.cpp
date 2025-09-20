////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/character/Character.hpp>
#include <mithril/character/CharacterCDB.hpp>
#include <mithril/character/CharacterJson.hpp>
#include <mithril/image/Image.hpp>
#include <yq/net/json.hpp>

namespace yq::mithril {
    json json_(Character x)
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
    
    json json_(const CharacterVector& xs)
    {
        json j = json::array();
        for(Character x : xs)
            j.push_back(json_(x));
        return j;
    }
}
