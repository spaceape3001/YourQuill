////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/book/Book.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon       qIcon(Book);
    
    namespace column {
        IdColumn  book_id(ColOpts opts=ColOpts());
        IdColumn  book_key(ColOpts opts=ColOpts());
        IdColumn  book_title(ColOpts opts=ColOpts());
    }

    namespace compareFN {
        IdColumn::CompareFN book_id();
        IdColumn::CompareFN book_key();
        IdColumn::CompareFN book_title();
    }

    namespace decorationFN {
        IdColumn::VariantFN  book_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  book_id();
        IdColumn::VariantFN  book_key();
        IdColumn::VariantFN  book_title();
    }
}

