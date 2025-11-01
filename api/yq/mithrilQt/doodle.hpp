////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/doodle/Doodle.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Doodle);
    
    namespace column {
        IdColumn  doodle_id(ColOpts opts=ColOpts());
        IdColumn  doodle_key(ColOpts opts=ColOpts());
        IdColumn  doodle_name(ColOpts opts=ColOpts());
    }
    namespace compareFN {
        IdColumn::CompareFN doodle_id();
        IdColumn::CompareFN doodle_key();
        IdColumn::CompareFN doodle_name();
    }
    namespace decorationFN {
        IdColumn::VariantFN  doodle_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  doodle_id();
        IdColumn::VariantFN  doodle_key();
        IdColumn::VariantFN  doodle_name();
    }
}

