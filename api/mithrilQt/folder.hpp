////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/folder/Folder.hpp>
#include <mithrilQt/id/IdColumn.hpp>

class QIcon;

namespace yq::mithril {
    QIcon   qIcon(Folder);
    namespace column {
        IdColumn  folder_id(ColOpts opts=ColOpts());
        IdColumn  folder_key(ColOpts opts=ColOpts());
        IdColumn  folder_name(ColOpts opts=ColOpts());
    }
    namespace decorationFN {
        IdColumn::VariantFN  folder_icon();
    }
    namespace displayFN {
        IdColumn::VariantFN  folder_id();
        IdColumn::VariantFN  folder_key();
        IdColumn::VariantFN  folder_name();
    }
}

