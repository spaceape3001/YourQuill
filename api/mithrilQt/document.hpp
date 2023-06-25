////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/document/Document.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Document);
    
    namespace column {
        IdColumn  document_id(ColOpts opts=ColOpts());
        IdColumn  document_key(ColOpts opts=ColOpts());
        IdColumn  document_name(ColOpts opts=ColOpts());
    }
    namespace decorationFN {
        IdColumn::VariantFN  document_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  document_id();
        IdColumn::VariantFN  document_key();
        IdColumn::VariantFN  document_name();
    }
}

