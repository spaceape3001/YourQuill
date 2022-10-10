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
    namespace update {

        /*! \brief Erases the tag
        */
        void                tag_erase(Tag);

        /*! \brief Erases the tag
        */
        void                tag_erase(Document);
        void                tag_icon(Tag);
        void                tag_notify(Fragment,Change);
        void                tag_notify_icons(Fragment,Change);
        void                tag_stage3(Document);
        void                tag_stage3_leaf(Document);
        Tag::SharedData     tag_update(Tag, cdb_options_t opts=0);
    }
}

