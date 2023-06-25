////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/attribute/Attribute.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Attribute);
    
    namespace column {
        IdColumn  attribute_id(ColOpts opts=ColOpts());
        IdColumn  attribute_key(ColOpts opts=ColOpts());
        IdColumn  attribute_value(ColOpts opts=ColOpts());
    }

    namespace decorationFN {
        IdColumn::VariantFN  attribute_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  attribute_id();
        IdColumn::VariantFN  attribute_key();
        IdColumn::VariantFN  attribute_value();
    }
}

