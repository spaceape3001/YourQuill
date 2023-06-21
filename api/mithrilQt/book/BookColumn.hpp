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
        IdColumn  book_id(ColOpts opts=ColOpts());
        IdColumn  book_key(ColOpts opts=ColOpts());
        IdColumn  book_title(ColOpts opts=ColOpts());
    }

    namespace displayFN {
        IdColumn::VariantFN  book_id();
        IdColumn::VariantFN  book_key();
        IdColumn::VariantFN  book_title();
    }
}

