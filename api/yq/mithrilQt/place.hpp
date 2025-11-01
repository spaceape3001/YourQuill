////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/place/Place.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Place);

    namespace column {
        IdColumn  place_id(ColOpts opts=ColOpts());
        IdColumn  place_key(ColOpts opts=ColOpts());
        IdColumn  place_name(ColOpts opts=ColOpts());
    }
    namespace compareFN {
        IdColumn::CompareFN place_id();
        IdColumn::CompareFN place_key();
        IdColumn::CompareFN place_name();
    }
    namespace decorationFN {
        IdColumn::VariantFN  place_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  place_id();
        IdColumn::VariantFN  place_key();
        IdColumn::VariantFN  place_name();
    }
}

