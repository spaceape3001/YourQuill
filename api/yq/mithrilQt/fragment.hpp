////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/fragment/Fragment.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon qIcon(Fragment);

    namespace column {
        IdColumn  fragment_id(ColOpts opts=ColOpts());
        IdColumn  fragment_key(ColOpts opts=ColOpts());
        IdColumn  fragment_name(ColOpts opts=ColOpts());
        IdColumn  fragment_path(ColOpts opts=ColOpts());
    }
    namespace compareFN {
        IdColumn::CompareFN fragment_id();
        IdColumn::CompareFN fragment_key();
        IdColumn::CompareFN fragment_name();
        IdColumn::CompareFN fragment_path();
    }
    namespace decorationFN {
        IdColumn::VariantFN  fragment_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  fragment_id();
        IdColumn::VariantFN  fragment_key();
        IdColumn::VariantFN  fragment_name();
        IdColumn::VariantFN  fragment_path();
    }
}

