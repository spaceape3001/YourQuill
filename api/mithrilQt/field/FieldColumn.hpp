////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdColumn.hpp>

namespace yq::mithril {
    namespace column {
        IdColumn  field_id(ColOpts opts=ColOpts());
        IdColumn  field_key(ColOpts opts=ColOpts());
        IdColumn  field_name(ColOpts opts=ColOpts());
    }
    namespace displayFN {
        IdColumn::VariantFN  field_id();
        IdColumn::VariantFN  field_key();
        IdColumn::VariantFN  field_name();
    }
}

