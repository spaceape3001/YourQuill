////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/field/Field.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Field);
    
    namespace column {
        IdColumn  field_id(ColOpts opts=ColOpts());
        IdColumn  field_key(ColOpts opts=ColOpts());
        IdColumn  field_name(ColOpts opts=ColOpts());
    }
    namespace decorationFN {
        IdColumn::VariantFN  field_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  field_id();
        IdColumn::VariantFN  field_key();
        IdColumn::VariantFN  field_name();
    }
}

