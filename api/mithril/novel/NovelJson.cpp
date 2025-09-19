////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include <mithril/novel/Novel.hpp>
#include <mithril/novel/NovelCDB.hpp>
#include <mithril/novel/NovelJson.hpp>
#include <yq/net/json.hpp>

namespace yq::mithril {
    json json_(Novel x)
    {
        Novel::Info  i   = cdb::info(x);
        json j{
            { "icon", i.icon.id },
            { "id", x.id },
            { "key", cdb::key(x) },
            { "title", i.title }
        };
        return j;
    }
    
    json json_(const NovelVector& xs)
    {
        json j = json::array();
        for(Novel x : xs)
            j.push_back(json_(x));
        return j;
    }
}
