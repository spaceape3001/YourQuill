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
        IdColumn  folder_id(ColOpts opts=ColOpts());
        IdColumn  folder_key(ColOpts opts=ColOpts());
        IdColumn  folder_name(ColOpts opts=ColOpts());
    }
    namespace displayFN {
        IdColumn::VariantFN  folder_id();
        IdColumn::VariantFN  folder_key();
        IdColumn::VariantFN  folder_name();
    }
}

