////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/graphviz/Graphviz.hpp>
#include <yq/mithril/graphviz/GraphvizCDB.hpp>
#include <yq/mithril/graphviz/GraphvizJson.hpp>
#include <yq/net/json.hpp>

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
