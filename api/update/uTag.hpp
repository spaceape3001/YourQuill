////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uFwd.hpp"
#include <kernel/org/Tag.hpp>

namespace yq {
    //! Updates the tag data
    Tag::SharedData         update(Tag, cdb_options_t opts=0);

    //! Updates Tag Icon
    void                    update_icon(Tag);
}
