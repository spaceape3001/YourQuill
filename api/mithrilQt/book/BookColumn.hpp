////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdColumn.hpp>

namespace yq::mithril {
    namespace column {
        IdColumn  book_id();
        IdColumn  book_key();
        IdColumn  book_title();
    }

    namespace displayFN {
        IdColumn::VariantFN  book_id();
        IdColumn::VariantFN  book_key();
        IdColumn::VariantFN  book_title();
    }
}

