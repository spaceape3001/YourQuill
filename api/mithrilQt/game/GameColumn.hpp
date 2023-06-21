////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdColumn.hpp>

namespace yq::mithril {
    namespace column {
        IdColumn  game_id();
        IdColumn  game_key();
        IdColumn  game_title();
    }
    namespace displayFN {
        IdColumn::VariantFN  game_id();
        IdColumn::VariantFN  game_key();
        IdColumn::VariantFN  game_title();
    }
}

