////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/graphviz/Graphviz.hpp>
#include <mithril/graphviz/GraphvizCDB.hpp>
#include <mithril/graphviz/GraphvizJson.hpp>

namespace yq::mithril {
    json json_(Graphviz x)
    {
        json j{
            { "id", x.id },
            { "key", cdb::key(x) },
            { "name", cdb::name(x) }
        };
        return j;
    }
    
    json json_(const GraphvizVector& xs)
    {
        json j = json::array();
        for(Graphviz x : xs)
            j.push_back(json_(x));
        return j;
    }
}
