////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/graphviz/Graphviz.hpp>
#include <mithril/graphviz/GraphvizCDB.hpp>
#include <mithril/graphviz/GraphvizProvider.hpp>

namespace yq::mithril::provider {
    IdProvider all_graphvizs()
    {
        return [](){
            return ids(cdb::all_graphvizs(Sorted::YES));
        };
    }
}

