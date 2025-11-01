////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/novel/Novel.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Novel);

    namespace column {
        IdColumn  novel_id(ColOpts opts=ColOpts());
        IdColumn  novel_key(ColOpts opts=ColOpts());
        IdColumn  novel_title(ColOpts opts=ColOpts());
    }
    namespace compareFN {
        IdColumn::CompareFN novel_id();
        IdColumn::CompareFN novel_key();
        IdColumn::CompareFN novel_title();
    }
    namespace decorationFN {
        IdColumn::VariantFN  novel_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  novel_id();
        IdColumn::VariantFN  novel_key();
        IdColumn::VariantFN  novel_title();
    }
}

