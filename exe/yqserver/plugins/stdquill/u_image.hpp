////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/CacheFwd.hpp>
#include <kernel/Change.hpp>

namespace yq {
    struct Image;
    struct Fragment;
    struct Document;

        //! Removes this image from the cache
    void    u_image_erase(Image);
        //! Removes this image from the cache
    void    u_image_erase(Fragment);
    void    u_image_notify(Fragment, Change);
    void    u_image_stage2();
    void    u_image_stage3(Document);
    void    u_image_thumbnails(Image img, cdb_options_t opts=0);
}
