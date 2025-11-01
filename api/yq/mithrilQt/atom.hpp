////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/atom/Atom.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Atom);
    QIcon   qIcon(Atom::Property);
    
    namespace column {
        IdColumn  atom_id(ColOpts opts=ColOpts());
        IdColumn  atom_key(ColOpts opts=ColOpts());
        IdColumn  atom_name(ColOpts opts=ColOpts());
    }
    
    namespace compareFN {
        IdColumn::CompareFN atom_id();
        IdColumn::CompareFN atom_key();
        IdColumn::CompareFN atom_name();
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

