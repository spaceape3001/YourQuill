////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdColumn.hpp>

namespace yq::mithril {
    namespace column {
        IdColumn  document_id();
        IdColumn  document_key();
        IdColumn  document_name();
    }
    namespace displayFN {
        IdColumn::VariantFN  document_id();
        IdColumn::VariantFN  document_key();
        IdColumn::VariantFN  document_name();
    }
}

