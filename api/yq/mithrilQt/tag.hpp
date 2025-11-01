////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/tag/Tag.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Tag);
    
    namespace column {
        IdColumn  tag_id(ColOpts opts=ColOpts());
        IdColumn  tag_key(ColOpts opts=ColOpts());
        IdColumn  tag_name(ColOpts opts=ColOpts());
    }
    namespace compareFN {
        IdColumn::CompareFN tag_id();
        IdColumn::CompareFN tag_key();
        IdColumn::CompareFN tag_name();
    }
    namespace decorationFN {
        IdColumn::VariantFN  tag_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  tag_id();
        IdColumn::VariantFN  tag_key();
        IdColumn::VariantFN  tag_name();
    }
}

