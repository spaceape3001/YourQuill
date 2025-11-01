////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/doodle/Doodle.hpp>
#include <yq/mithril/doodle/DoodleCDB.hpp>
#include <yq/mithril/doodle/DoodleInfo.hpp>
#include <yq/mithril/doodle/DoodleJson.hpp>
#include <yq/net/json.hpp>

namespace yq::mithril {
    json json_(Doodle x)
    {
        Doodle::Info  i   = cdb::info(x);
        json j{
            { "id", x.id },
            { "key", cdb::key(x) },
            { "name", i.name }
        };
        return j;
    }
    
    json json_(const DoodleVector& xs)
    {
        json j = json::array();
        for(Doodle x : xs)
            j.push_back(json_(x));
        return j;
    }
}
