////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/style/Style.hpp>
#include <yq/mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Style);
    
    namespace column {
        IdColumn  style_id(ColOpts opts=ColOpts());
        IdColumn  style_key(ColOpts opts=ColOpts());
        IdColumn  style_name(ColOpts opts=ColOpts());
    }
    namespace compareFN {
        IdColumn::CompareFN style_id();
        IdColumn::CompareFN style_key();
        IdColumn::CompareFN style_name();
    }
    namespace decorationFN {
        IdColumn::VariantFN  style_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  style_id();
        IdColumn::VariantFN  style_key();
        IdColumn::VariantFN  style_name();
    }
}

