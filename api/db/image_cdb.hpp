////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "cdb_common.hpp"
#include "enum_size_desc.hpp"
#include "image.hpp"
#include <yq/net/Http.hpp>

namespace yq {
    namespace cdb {
            //! Returns a vector of all images in the cache
        Vector<Image>           all_images();
        
            //! Returns a count of the available images
        size_t                  all_images_count();

            //! Returns the image bytes (either from file or database)
        ByteArray               bytes(Image, SizeDesc sz=SizeDesc::Original);

            //! Creates/Retrieves an image record in the cache database
        Image                   db_image(Fragment, bool *wasCreated=nullptr);

        Document                document(Image);

        bool                    exists(Image);
        bool                    exists_image(uint64_t);
        

        Fragment                fragment(Image);

        Image                   icon(const Root*);

            //! Returns an image from a document, if it already exists in the cache
        Image                   image(Document);
        Image                   image(Fragment);

        Image                   image(std::string_view);

            //! Returns an image from a document, if it already exists in the cache
        Image                   image(uint64_t);

        bool                    raster(ContentType);
        bool                    raster(Image);

        ContentType             type(Image);
        void                    update_root(const Root*, Image);
    }
}
