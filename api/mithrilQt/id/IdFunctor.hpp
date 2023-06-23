////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

/*
    Because the id-column is already taken as a name, this will be here for 
    a more general purpose set of functions...
*/


#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdColumn.hpp>

namespace yq::mithril {
    namespace column {
        IdColumn                id_id(ColOpts opts=ColOpts());
        IdColumn                id_key(ColOpts opts=ColOpts());
        IdColumn                id_label(ColOpts opts=ColOpts());
    }

    namespace decorationFN {
        IdColumn::VariantFN     id_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN     id_id();
        IdColumn::VariantFN     id_key();
        IdColumn::VariantFN     id_label();
    }
}
