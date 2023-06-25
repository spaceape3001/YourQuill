////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/class/Class.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon qIcon(Class);
    
    namespace column {
        IdColumn  class_id(ColOpts opts=ColOpts());
        IdColumn  class_key(ColOpts opts=ColOpts());
        IdColumn  class_name(ColOpts opts=ColOpts());
    }
    namespace decorationFN {
        IdColumn::VariantFN  class_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  class_id();
        IdColumn::VariantFN  class_key();
        IdColumn::VariantFN  class_name();
    }
}

