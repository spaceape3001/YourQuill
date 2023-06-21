////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdColumn.hpp>

namespace yq::mithril {
    namespace column {
        IdColumn  directory_hidden();
        IdColumn  directory_id();
        IdColumn  directory_key();
        IdColumn  directory_name();
        IdColumn  directory_path();
    }

    namespace displayFN {
        IdColumn::VariantFN  directory_hidden();
        IdColumn::VariantFN  directory_id();
        IdColumn::VariantFN  directory_key();
        IdColumn::VariantFN  directory_name();
        IdColumn::VariantFN  directory_path();
    }
}

