////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/event/Event.hpp>
#include <yq/mithril/event/EventCDB.hpp>
#include <yq/mithril/event/EventJson.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/net/json.hpp>

namespace yq::mithril {
    json json_(Event x)
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
    
    json json_(const EventVector& xs)
    {
        json j = json::array();
        for(Event x : xs)
            j.push_back(json_(x));
        return j;
    }
}
