////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uFwd.hpp"
#include <kernel/org/Tag.hpp>

namespace yq {
    struct UTag : trait::not_copyable, trait::not_moveable {
        const std::string       k;
        union {
            const uint64_t      id;
            const Tag           tag;
            const Document      doc;
        };
        
        explicit UTag(Tag);
    };

    UTag&  uget(Tag);
    
    //! Updates the tag data
    Tag::SharedData         update(Tag, cdb_options_t opts=0);

    //! Updates Tag Icon
    void                    update_icon(Tag);
}
