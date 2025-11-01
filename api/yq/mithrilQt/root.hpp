////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/root/Root.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon qIcon(Root);

    namespace column {
        IdColumn  root_id(ColOpts opts=ColOpts());
        IdColumn  root_key(ColOpts opts=ColOpts());
        IdColumn  root_name(ColOpts opts=ColOpts());
        IdColumn  root_path(ColOpts opts=ColOpts());
        IdColumn  root_template(ColOpts opts=ColOpts());
    }
    namespace compareFN {
        IdColumn::CompareFN     root_id();
        IdColumn::CompareFN     root_key();
        IdColumn::CompareFN     root_name();
        IdColumn::CompareFN     root_path();
        IdColumn::CompareFN     root_template();
    }
    namespace decorationFN {
        IdColumn::VariantFN  root_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  root_id();
        IdColumn::VariantFN  root_key();
        IdColumn::VariantFN  root_name();
        IdColumn::VariantFN  root_path();
        IdColumn::VariantFN  root_template();
    }
}

