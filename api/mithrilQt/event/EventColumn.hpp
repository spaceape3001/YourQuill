////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdColumn.hpp>

namespace yq::mithril {
    namespace column {
        IdColumn  event_id();
        IdColumn  event_key();
        IdColumn  event_title();
    }

    namespace displayFN {
        IdColumn::VariantFN  event_id();
        IdColumn::VariantFN  event_key();
        IdColumn::VariantFN  event_title();
    }
}

