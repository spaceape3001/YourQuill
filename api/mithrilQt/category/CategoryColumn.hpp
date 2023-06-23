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
    QIcon   qIcon(Category);
    namespace column {
        IdColumn  category_id(ColOpts opts=ColOpts());
        IdColumn  category_key(ColOpts opts=ColOpts());
        IdColumn  category_name(ColOpts opts=ColOpts());
    }
    namespace decorationFN {
        IdColumn::VariantFN  category_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  category_id();
        IdColumn::VariantFN  category_key();
        IdColumn::VariantFN  category_name();
    }
}

