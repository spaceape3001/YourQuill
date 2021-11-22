////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uFwd.hpp"
#include <db/Tag.hpp>

struct UTag {
    const String            key;
    union {
        const uint64_t      id;
        const Tag           tag;
        const Document      doc;
    };
    TagData::Shared         data;
    Image                   icon;
    
    static void         init_read();
    static void         init_link();
    static void         on_tag_added(Fragment);
    static void         on_tag_change(Fragment);
    static void         on_image_change(Fragment);
    
    UTag(Tag);

    //! Deterimes the explicit icon for this tag
    Image               explicit_icon() const;
    
    // Disable these
    ~UTag() = delete;
    UTag(const UTag&) = delete;
    UTag(UTag&&) = delete;
    UTag& operator=(const UTag&) = delete;
    UTag& operator=(UTag&&) = delete;

    void    do_icon();
    void    do_read();
    void    do_link();
};

UTag&       uget(Tag);


