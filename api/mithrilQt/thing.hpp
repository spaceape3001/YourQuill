////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/thing/Thing.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Thing);

    namespace column {
        IdColumn  thing_id(ColOpts opts=ColOpts());
        IdColumn  thing_key(ColOpts opts=ColOpts());
        IdColumn  thing_name(ColOpts opts=ColOpts());
    }
    namespace compareFN {
        IdColumn::CompareFN thing_id();
        IdColumn::CompareFN thing_key();
        IdColumn::CompareFN thing_name();
    }
    namespace decorationFN {
        IdColumn::VariantFN  thing_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  thing_id();
        IdColumn::VariantFN  thing_key();
        IdColumn::VariantFN  thing_name();
    }
}

