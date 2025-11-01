////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/attribute/Attribute.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Attribute);
    
    namespace column {
        IdColumn  attribute_id(ColOpts opts=ColOpts());
        IdColumn  attribute_key(ColOpts opts=ColOpts());
        IdColumn  attribute_value(ColOpts opts=ColOpts());
    }

    namespace compareFN {
        IdColumn::CompareFN attribute_id();
        IdColumn::CompareFN attribute_key();
        IdColumn::CompareFN attribute_value();
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

