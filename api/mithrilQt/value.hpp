////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/value/Value.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Value);
    
    namespace column {
        IdColumn  value_id(ColOpts opts=ColOpts());
        IdColumn  value_key(ColOpts opts=ColOpts());
        IdColumn  value_name(ColOpts opts=ColOpts());
        IdColumn  value_value(ColOpts opts=ColOpts());
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

