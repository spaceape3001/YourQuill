////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/leaf/Leaf.hpp>
#include <mithril/leaf/LeafCDB.hpp>
#include <mithril/leaf/LeafJson.hpp>

namespace yq::mithril {
    json json_(Leaf x)
    {
        Leaf::Info  i   = cdb::info(x);
        json j{
            { "brief", i.brief },
            { "document", i.doc.id },
            { "icon", i.icon.id },
            { "id", x.id },
            { "key", i.key },
            { "title", i.title }
        };
        return j;
    }
    
    json json_(const LeafVector& xs)
    {
        json j = json::array();
        for(Leaf x : xs)
            j.push_back(json_(x));
        return j;
    }
}
