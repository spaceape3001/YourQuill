////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uFwd.hpp"
#include <db/Tag.hpp>

struct UTag {
    const QString           key;
    union {
        const uint64_t      id;
        const Tag           tag;
        const Document      doc;
    };
    
    UTag(Tag);

    //! Deterimes the explicit icon for this tag
    Image               explicit_icon() const;
};

UTag&       uget(Tag);


