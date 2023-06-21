////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdColumn.hpp>

namespace yq::mithril {
    namespace column {
        IdColumn  fragment_id();
        IdColumn  fragment_key();
        IdColumn  fragment_name();
        IdColumn  fragment_path();
    }
    namespace displayFN {
        IdColumn::VariantFN  fragment_id();
        IdColumn::VariantFN  fragment_key();
        IdColumn::VariantFN  fragment_name();
        IdColumn::VariantFN  fragment_path();
    }
}

