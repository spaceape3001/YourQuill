////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/image/ImageProvider.hpp>
#include <mithril/image/ImageCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_images()
    {
        return [](){
            return ids(cdb::all_images(Sorted::YES));
        };
    }
}

