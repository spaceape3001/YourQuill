////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CacheFwd.hpp"
#include <db/enum/SizeDesc.hpp>
#include <yq/net/Http.hpp>

class QIcon;

namespace yq {

    struct Image {
        static constexpr const int kSmallWidth     =  24;
        static constexpr const int kSmallHeight    =  32;
        static constexpr const int kMediumWidth    =  64;
        static constexpr const int kMediumHeight   =  48;
        static constexpr const int kLargeWidth     = 128;
        static constexpr const int kLargeHeight    =  96;
        
        static constexpr const std::initializer_list<const char*>   kSupportedExtensions = {
            "svg", "png", "gif", "jpg", "bmp", "tif", "tiff" 
        };

        uint64_t  id = 0ULL;
        constexpr auto    operator<=>(const Image& rhs) const = default;
        operator bool() const { return id != 0ULL; }
    };

    namespace cdb {
            //! Returns a vector of all images in the cache
        Vector<Image>           all_images();
        
            //! Returns a count of the available images
        size_t                  all_images_count();

            //! Returns the image bytes (either from file or database)
        QByteArray              bytes(Image, SizeDesc sz=SizeDesc::Original);

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

        Image                   image(const String&);

            //! Returns an image from a document, if it already exists in the cache
        Image                   image(uint64_t);

        QIcon                   qIcon(Image);

        bool                    raster(ContentType);
        bool                    raster(Image);

        ContentType             type(Image);
        void                    update_root(const Root*, Image);
    }

}
