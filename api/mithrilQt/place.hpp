////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/place/Place.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Place);

    namespace column {
        IdColumn  place_id(ColOpts opts=ColOpts());
        IdColumn  place_key(ColOpts opts=ColOpts());
        IdColumn  place_name(ColOpts opts=ColOpts());
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

