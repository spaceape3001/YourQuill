////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdColumn.hpp>

namespace yq::mithril {
    namespace column {
        IdColumn  leaf_id();
        IdColumn  leaf_key();
        IdColumn  leaf_title();
    }
    namespace displayFN {
        IdColumn::VariantFN  leaf_id();
        IdColumn::VariantFN  leaf_key();
        IdColumn::VariantFN  leaf_title();
    }
}

