////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdColumn.hpp>

namespace yq::mithril {
    struct IdColumn;
    namespace column {
        IdColumn  category_id(ColOpts opts=ColOpts());
        IdColumn  category_key(ColOpts opts=ColOpts());
        IdColumn  category_name(ColOpts opts=ColOpts());
    }
    namespace displayFN {
        IdColumn::VariantFN  category_id();
        IdColumn::VariantFN  category_key();
        IdColumn::VariantFN  category_name();
    }
}

