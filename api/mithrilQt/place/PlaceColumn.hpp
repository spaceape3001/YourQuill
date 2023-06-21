////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdColumn.hpp>

namespace yq::mithril {
    namespace column {
        IdColumn  place_id();
        IdColumn  place_key();
        IdColumn  place_name();
    }
    namespace displayFN {
        IdColumn::VariantFN  place_id();
        IdColumn::VariantFN  place_key();
        IdColumn::VariantFN  place_name();
    }
}

