////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/image/ImageProvider.hpp>
#include <yq/mithril/image/ImageCDB.hpp>

namespace yq::mithril::provider {
    IdProvider all_images()
    {
        return [](){
            return ids(cdb::all_images(Sorted::YES));
        };
    }
}

