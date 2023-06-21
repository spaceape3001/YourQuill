////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdColumn.hpp>

namespace yq::mithril {
    struct IdColumn;
    namespace column {
        IdColumn  character_id();
        IdColumn  character_key();
        IdColumn  character_name();
    }
    namespace displayFN {
        IdColumn::VariantFN  character_id();
        IdColumn::VariantFN  character_key();
        IdColumn::VariantFN  character_name();
    }
}

