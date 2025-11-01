////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/graphviz/Graphviz.hpp>
#include <yq/mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Graphviz);
    
    namespace column {
        IdColumn  graphviz_id(ColOpts opts=ColOpts());
        IdColumn  graphviz_key(ColOpts opts=ColOpts());
        IdColumn  graphviz_name(ColOpts opts=ColOpts());
    }
    namespace compareFN {
        IdColumn::CompareFN graphviz_id();
        IdColumn::CompareFN graphviz_key();
        IdColumn::CompareFN graphviz_name();
    }
    namespace decorationFN {
        IdColumn::VariantFN  graphviz_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  graphviz_id();
        IdColumn::VariantFN  graphviz_key();
        IdColumn::VariantFN  graphviz_name();
    }
}

