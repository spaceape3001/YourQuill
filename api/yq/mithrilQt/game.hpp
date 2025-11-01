////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/game/Game.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Game);
    
    namespace column {
        IdColumn  game_id(ColOpts opts=ColOpts());
        IdColumn  game_key(ColOpts opts=ColOpts());
        IdColumn  game_title(ColOpts opts=ColOpts());
    }
    namespace compareFN {
        IdColumn::CompareFN game_id();
        IdColumn::CompareFN game_key();
        IdColumn::CompareFN game_title();
    }
    namespace decorationFN {
        IdColumn::VariantFN  game_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  game_id();
        IdColumn::VariantFN  game_key();
        IdColumn::VariantFN  game_title();
    }
}

