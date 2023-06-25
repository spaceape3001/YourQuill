////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/event/Event.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Event);
    namespace column {
        IdColumn  event_id(ColOpts opts=ColOpts());
        IdColumn  event_key(ColOpts opts=ColOpts());
        IdColumn  event_title(ColOpts opts=ColOpts());
    }
    namespace decorationFN {
        IdColumn::VariantFN  event_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  event_id();
        IdColumn::VariantFN  event_key();
        IdColumn::VariantFN  event_title();
    }
}

