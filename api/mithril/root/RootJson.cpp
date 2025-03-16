////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/root/Root.hpp>
#include <mithril/root/RootDir.hpp>
#include <mithril/root/RootCDB.hpp>
#include <mithril/root/RootJson.hpp>
#include <nlohmann/json.hpp>

namespace yq::mithril {
    json json_(const RootDir*rt)
    {
        json    j;
        if(rt){
            j["color"]  = rt->color;
            j["id"]     = rt->id;
            j["depth"]  = rt->depth;
            j["key"]    = rt->key;
            j["name"]   = rt->name;
            j["path"]   = rt->path.string();
        }
        return j;
    }
    
    json json_(const RootDirVector&xs)
    {
        json j = json::array();
        for(const RootDir* x : xs)
            j.push_back(json_(x));
        return j;
    }
    
}
