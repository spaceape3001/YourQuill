////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdColumn.hpp>

namespace yq::mithril {
    struct IdColumn;
    namespace column {
        IdColumn  category_id();
        IdColumn  category_key();
        IdColumn  category_name();
    }
    namespace displayFN {
        IdColumn::VariantFN  category_id();
        IdColumn::VariantFN  category_key();
        IdColumn::VariantFN  category_name();
    }
}

