////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/graph/Graph.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Graph);
    
    namespace column {
        IdColumn  graph_id(ColOpts opts=ColOpts());
        IdColumn  graph_key(ColOpts opts=ColOpts());
        IdColumn  graph_name(ColOpts opts=ColOpts());
    }
    namespace compareFN {
        IdColumn::CompareFN graph_id();
        IdColumn::CompareFN graph_key();
        IdColumn::CompareFN graph_name();
    }
    namespace decorationFN {
        IdColumn::VariantFN  graph_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  graph_id();
        IdColumn::VariantFN  graph_key();
        IdColumn::VariantFN  graph_name();
    }
}

