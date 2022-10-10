////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/db/CacheFwd.hpp>
#include <kernel/enum/Change.hpp>

namespace yq {
    struct Image;
    struct Fragment;
    struct Document;

    namespace update {
        void    image_notify(Fragment, Change);
        void    image_stage2();
        void    image_stage3(Document);
        void    image_thumbnails(Image img, cdb_options_t opts=0);
    }
}
