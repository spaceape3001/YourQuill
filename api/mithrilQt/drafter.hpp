////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/drafter/Drafter.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Drafter);
    
    namespace column {
        IdColumn  drafter_id(ColOpts opts=ColOpts());
        IdColumn  drafter_key(ColOpts opts=ColOpts());
        IdColumn  drafter_name(ColOpts opts=ColOpts());
    }
    namespace compareFN {
        IdColumn::CompareFN drafter_id();
        IdColumn::CompareFN drafter_key();
        IdColumn::CompareFN drafter_name();
    }
    namespace decorationFN {
        IdColumn::VariantFN  drafter_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  drafter_id();
        IdColumn::VariantFN  drafter_key();
        IdColumn::VariantFN  drafter_name();
    }
}

