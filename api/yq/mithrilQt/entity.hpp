////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/entity/Entity.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Entity);
    namespace column {
        IdColumn  entity_id(ColOpts opts=ColOpts());
        IdColumn  entity_key(ColOpts opts=ColOpts());
        IdColumn  entity_name(ColOpts opts=ColOpts());
    }
    namespace compareFN {
        IdColumn::CompareFN entity_id();
        IdColumn::CompareFN entity_key();
        IdColumn::CompareFN entity_name();
    }
    namespace decorationFN {
        IdColumn::VariantFN  entity_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  entity_id();
        IdColumn::VariantFN  entity_key();
        IdColumn::VariantFN  entity_name();
    }
}

