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
        IdColumn  tag_id(ColOpts opts=ColOpts());
        IdColumn  tag_key(ColOpts opts=ColOpts());
        IdColumn  tag_name(ColOpts opts=ColOpts());
    }
    namespace decorationFN {
        IdColumn::VariantFN  tag_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  tag_id();
        IdColumn::VariantFN  tag_key();
        IdColumn::VariantFN  tag_name();
    }
}

