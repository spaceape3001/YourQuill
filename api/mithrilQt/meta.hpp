////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/meta/Meta.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Meta);

    namespace column {
        IdColumn  meta_id(ColOpts opts=ColOpts());
        IdColumn  meta_key(ColOpts opts=ColOpts());
        IdColumn  meta_name(ColOpts opts=ColOpts());
    }
    namespace compareFN {
        IdColumn::CompareFN meta_id();
        IdColumn::CompareFN meta_key();
        IdColumn::CompareFN meta_name();
    }
    namespace decorationFN {
        IdColumn::VariantFN  meta_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  meta_id();
        IdColumn::VariantFN  meta_key();
        IdColumn::VariantFN  meta_name();
    }
}

