////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/atom/AtomCDB.hpp>
#include <mithril/atom/AtomInfo.hpp>
#include <mithril/atom/AtomJson.hpp>
#include <mithril/image/Image.hpp>

#include <yq/typedef/json.hpp>
#include <yq/net/json.hpp>

namespace yq::mithril {
    json json_(Atom x)
    {
        Atom::Info  i   = cdb::info(x);
        json j{
            { "abbr", i.abbr },
            { "brief", i.brief },
            { "icon", i.icon.id },
            { "id", x.id },
            { "key", i.key },
            { "name", i.name }
        };
        
        if(i.leaf)
            j["leaf"]   = i.leaf.id;
        
        if(i.parent)
            j["parent"] = i.parent.id;
        
        return j;
    }

    json json_(const AtomVector& xs)
    {
        json j = json::array();
        for(Atom x : xs)
            j.push_back(json_(x));
        return j;
    }
}
