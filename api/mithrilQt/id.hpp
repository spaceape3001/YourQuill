////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon       id_qIcon(Id);

    namespace column {
        IdColumn  id_id(ColOpts opts=ColOpts());
        IdColumn  id_key(ColOpts opts=ColOpts());
        IdColumn  id_name(ColOpts opts=ColOpts());
        IdColumn  id_type(ColOpts opts=ColOpts());
    }

    namespace decorationFN {
        IdColumn::VariantFN  id_icon();
    }

    namespace displayFN {
        IdColumn::VariantFN  id_id();
        IdColumn::VariantFN  id_key();
        IdColumn::VariantFN  id_name();
        IdColumn::VariantFN  id_type();
    }
}

