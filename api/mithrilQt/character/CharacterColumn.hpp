////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Character);
    namespace column {
        IdColumn  character_id(ColOpts opts=ColOpts());
        IdColumn  character_key(ColOpts opts=ColOpts());
        IdColumn  character_name(ColOpts opts=ColOpts());
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

