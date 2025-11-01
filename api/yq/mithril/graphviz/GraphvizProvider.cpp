////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/graphviz/Graphviz.hpp>
#include <yq/mithril/graphviz/GraphvizCDB.hpp>
#include <yq/mithril/graphviz/GraphvizProvider.hpp>

namespace yq::mithril::provider {
    IdProvider all_graphvizs()
    {
        return [](){
            return ids(cdb::all_graphvizs(Sorted::YES));
        };
    }
}

