////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/root/Root.hpp>
#include <yq/mithril/root/RootDir.hpp>
#include <yq/mithril/root/RootCDB.hpp>
#include <yq/mithril/root/RootJson.hpp>
#include <yq/net/json.hpp>

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
