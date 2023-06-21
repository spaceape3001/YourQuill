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
        IdColumn  atom_id(ColOpts opts=ColOpts());
        IdColumn  atom_key(ColOpts opts=ColOpts());
        IdColumn  atom_name(ColOpts opts=ColOpts());
    }

    namespace displayFN {
        IdColumn::VariantFN  atom_id();
        IdColumn::VariantFN  atom_key();
        IdColumn::VariantFN  atom_name();
    }
}

