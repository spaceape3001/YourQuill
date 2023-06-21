////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdColumn.hpp>

namespace yq::mithril {
    namespace column {
        IdColumn  root_id();
        IdColumn  root_key();
        IdColumn  root_name();
        IdColumn  root_path();
        IdColumn  root_template();
    }
    namespace displayFN {
        IdColumn::VariantFN  root_id();
        IdColumn::VariantFN  root_key();
        IdColumn::VariantFN  root_name();
        IdColumn::VariantFN  root_path();
        IdColumn::VariantFN  root_template();
    }
}

