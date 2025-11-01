////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/user/User.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(User);
    
    namespace column {
        IdColumn  user_id(ColOpts opts=ColOpts());
        IdColumn  user_key(ColOpts opts=ColOpts());
        IdColumn  user_name(ColOpts opts=ColOpts());
    }
    namespace compareFN {
        IdColumn::CompareFN user_id();
        IdColumn::CompareFN user_key();
        IdColumn::CompareFN user_name();
    }
    namespace decorationFN {
        IdColumn::VariantFN  user_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  user_id();
        IdColumn::VariantFN  user_key();
        IdColumn::VariantFN  user_name();
    }
}

