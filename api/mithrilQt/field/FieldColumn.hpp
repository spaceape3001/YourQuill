////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdColumn.hpp>

namespace yq::mithril {
    namespace column {
        IdColumn  field_id();
        IdColumn  field_key();
        IdColumn  field_name();
    }
    namespace displayFN {
        IdColumn::VariantFN  field_id();
        IdColumn::VariantFN  field_key();
        IdColumn::VariantFN  field_name();
    }
}

