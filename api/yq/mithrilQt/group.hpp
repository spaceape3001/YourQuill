////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/group/Group.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Group);
    namespace column {
        IdColumn  group_id(ColOpts opts=ColOpts());
        IdColumn  group_key(ColOpts opts=ColOpts());
        IdColumn  group_name(ColOpts opts=ColOpts());
    }
    namespace compareFN {
        IdColumn::CompareFN group_id();
        IdColumn::CompareFN group_key();
        IdColumn::CompareFN group_name();
    }
    namespace decorationFN {
        IdColumn::VariantFN  group_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  group_id();
        IdColumn::VariantFN  group_key();
        IdColumn::VariantFN  group_name();
    }
}

