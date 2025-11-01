////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/category/Category.hpp>
#include <yq/mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Category);
    namespace column {
        IdColumn  category_id(ColOpts opts=ColOpts());
        IdColumn  category_key(ColOpts opts=ColOpts());
        IdColumn  category_name(ColOpts opts=ColOpts());
    }
    namespace compareFN {
        IdColumn::CompareFN category_id();
        IdColumn::CompareFN category_key();
        IdColumn::CompareFN category_name();
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

