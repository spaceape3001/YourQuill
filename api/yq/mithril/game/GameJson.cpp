////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/game/Game.hpp>
#include <yq/mithril/game/GameCDB.hpp>
#include <yq/mithril/game/GameJson.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/net/json.hpp>

namespace yq::mithril {
    json json_(Game x)
    {
        json j{
            { "brief", cdb::brief(x) },
            { "icon", cdb::icon(x).id },
            { "id", x.id },
            { "key", cdb::key(x) },
            { "title", cdb::title(x) }
        };
        return j;
    }
    
    json json_(const GameVector& xs)
    {
        json j = json::array();
        for(Game x : xs)
            j.push_back(json_(x));
        return j;
    }
}
