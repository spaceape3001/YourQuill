////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Atom);
    
    namespace column {
        IdColumn  atom_id(ColOpts opts=ColOpts());
        IdColumn  atom_key(ColOpts opts=ColOpts());
        IdColumn  atom_name(ColOpts opts=ColOpts());
    }

    namespace decorationFN {
        IdColumn::VariantFN  atom_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  atom_id();
        IdColumn::VariantFN  atom_key();
        IdColumn::VariantFN  atom_name();
    }
}

