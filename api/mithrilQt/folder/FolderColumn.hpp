////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdColumn.hpp>

namespace yq::mithril {
    namespace column {
        IdColumn  folder_id();
        IdColumn  folder_key();
        IdColumn  folder_name();
    }
    namespace displayFN {
        IdColumn::VariantFN  folder_id();
        IdColumn::VariantFN  folder_key();
        IdColumn::VariantFN  folder_name();
    }
}

