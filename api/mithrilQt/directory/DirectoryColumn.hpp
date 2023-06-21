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
        IdColumn  directory_hidden(ColOpts opts=ColOpts());
        IdColumn  directory_id(ColOpts opts=ColOpts());
        IdColumn  directory_key(ColOpts opts=ColOpts());
        IdColumn  directory_name(ColOpts opts=ColOpts());
        IdColumn  directory_path(ColOpts opts=ColOpts());
    }

    namespace displayFN {
        IdColumn::VariantFN  directory_hidden();
        IdColumn::VariantFN  directory_id();
        IdColumn::VariantFN  directory_key();
        IdColumn::VariantFN  directory_name();
        IdColumn::VariantFN  directory_path();
    }
}

