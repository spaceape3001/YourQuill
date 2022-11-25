////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/db/CacheFwd.hpp>
#include <kernel/enum/Change.hpp>
#include <kernel/org/Tag.hpp>

namespace yq {
    struct Document;
    struct Fragment;

    /*! \brief Erases the tag
    */
    void                u_tag_erase(Tag);

    /*! \brief Erases the tag
    */
    void                u_tag_erase(Document);
    void                u_tag_icon(Tag);
    void                u_tag_notify(Fragment,Change);
    void                u_tag_notify_icons(Fragment,Change);
    void                u_tag_stage3(Document);
    void                u_tag_stage3_leaf(Document);
    Tag::SharedData     u_tag_update(Tag, cdb_options_t opts=0);
}

