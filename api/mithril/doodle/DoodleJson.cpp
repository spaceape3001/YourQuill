////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/doodle/Doodle.hpp>
#include <mithril/doodle/DoodleCDB.hpp>
#include <mithril/doodle/DoodleInfo.hpp>
#include <mithril/doodle/DoodleJson.hpp>
#include <nlohmann/json.hpp>

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
