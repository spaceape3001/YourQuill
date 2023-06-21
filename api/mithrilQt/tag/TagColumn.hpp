////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdColumn.hpp>

namespace yq::mithril {
    namespace column {
        IdColumn  tag_id();
        IdColumn  tag_key();
        IdColumn  tag_name();
    }
    namespace displayFN {
        IdColumn::VariantFN  tag_id();
        IdColumn::VariantFN  tag_key();
        IdColumn::VariantFN  tag_name();
    }
}

