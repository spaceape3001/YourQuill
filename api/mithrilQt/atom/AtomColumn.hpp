////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdColumn.hpp>

namespace yq::mithril {
    namespace column {
        IdColumn  atom_id();
        IdColumn  atom_key();
        IdColumn  atom_name();
    }

    namespace displayFN {
        IdColumn::VariantFN  atom_id();
        IdColumn::VariantFN  atom_key();
        IdColumn::VariantFN  atom_name();
    }
}

