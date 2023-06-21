////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdColumn.hpp>

namespace yq::mithril {
    namespace column {
        IdColumn  fragment_id(ColOpts opts=ColOpts());
        IdColumn  fragment_key(ColOpts opts=ColOpts());
        IdColumn  fragment_name(ColOpts opts=ColOpts());
        IdColumn  fragment_path(ColOpts opts=ColOpts());
    }
    namespace displayFN {
        IdColumn::VariantFN  fragment_id();
        IdColumn::VariantFN  fragment_key();
        IdColumn::VariantFN  fragment_name();
        IdColumn::VariantFN  fragment_path();
    }
}

