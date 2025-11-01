////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/value/Value.hpp>
#include <yq/mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Value);
    
    namespace column {
        IdColumn  value_id(ColOpts opts=ColOpts());
        IdColumn  value_key(ColOpts opts=ColOpts());
        IdColumn  value_name(ColOpts opts=ColOpts());
        IdColumn  value_value(ColOpts opts=ColOpts());
    }
    namespace compareFN {
        IdColumn::CompareFN value_id();
        IdColumn::CompareFN value_key();
        IdColumn::CompareFN value_name();
        IdColumn::CompareFN value_value();
    }
    namespace decorationFN {
        IdColumn::VariantFN  value_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  value_id();
        IdColumn::VariantFN  value_key();
        IdColumn::VariantFN  value_name();
        IdColumn::VariantFN  value_value();
    }
}

