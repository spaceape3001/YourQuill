////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/organization/Organization.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Organization);
    namespace column {
        IdColumn  organization_id(ColOpts opts=ColOpts());
        IdColumn  organization_key(ColOpts opts=ColOpts());
        IdColumn  organization_name(ColOpts opts=ColOpts());
    }
    namespace compareFN {
        IdColumn::CompareFN organization_id();
        IdColumn::CompareFN organization_key();
        IdColumn::CompareFN organization_name();
    }
    namespace decorationFN {
        IdColumn::VariantFN  organization_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  organization_id();
        IdColumn::VariantFN  organization_key();
        IdColumn::VariantFN  organization_name();
    }
}

