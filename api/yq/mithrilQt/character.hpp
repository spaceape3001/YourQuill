////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/character/Character.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Character);
    namespace column {
        IdColumn  character_id(ColOpts opts=ColOpts());
        IdColumn  character_key(ColOpts opts=ColOpts());
        IdColumn  character_name(ColOpts opts=ColOpts());
    }
    namespace compareFN {
        IdColumn::CompareFN character_id();
        IdColumn::CompareFN character_key();
        IdColumn::CompareFN character_name();
    }
    namespace decorationFN {
        IdColumn::VariantFN  character_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  character_id();
        IdColumn::VariantFN  character_key();
        IdColumn::VariantFN  character_name();
    }
}

