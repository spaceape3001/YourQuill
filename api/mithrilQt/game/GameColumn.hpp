////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdColumn.hpp>

namespace yq::mithril {
    namespace column {
        IdColumn  game_id(ColOpts opts=ColOpts());
        IdColumn  game_key(ColOpts opts=ColOpts());
        IdColumn  game_title(ColOpts opts=ColOpts());
    }
    namespace displayFN {
        IdColumn::VariantFN  game_id();
        IdColumn::VariantFN  game_key();
        IdColumn::VariantFN  game_title();
    }
}

