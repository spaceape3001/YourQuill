////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/leaf/Leaf.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Leaf);

    namespace column {
        IdColumn  leaf_id(ColOpts opts=ColOpts());
        IdColumn  leaf_key(ColOpts opts=ColOpts());
        IdColumn  leaf_title(ColOpts opts=ColOpts());
    }
    namespace compareFN {
        IdColumn::CompareFN leaf_id();
        IdColumn::CompareFN leaf_key();
        IdColumn::CompareFN leaf_title();
    }
    namespace decorationFN {
        IdColumn::VariantFN  leaf_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  leaf_id();
        IdColumn::VariantFN  leaf_key();
        IdColumn::VariantFN  leaf_title();
    }
}

