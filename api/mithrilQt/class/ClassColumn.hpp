////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdColumn.hpp>

namespace yq::mithril {
    namespace column {
        IdColumn  class_id();
        IdColumn  class_key();
        IdColumn  class_name();
    }
    namespace displayFN {
        IdColumn::VariantFN  class_id();
        IdColumn::VariantFN  class_key();
        IdColumn::VariantFN  class_name();
    }
}

