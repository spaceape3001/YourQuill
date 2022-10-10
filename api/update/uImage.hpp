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
            //! Removes this image from the cache
        void    image_erase(Image);
            //! Removes this image from the cache
        void    image_erase(Fragment);
        void    image_notify(Fragment, Change);
        void    image_stage2();
        void    image_stage3(Document);
        void    image_thumbnails(Image img, cdb_options_t opts=0);
    }
}
