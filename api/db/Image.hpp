////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SizeDesc.hpp"
#include "util/Http.hpp"
#include "CacheFwd.hpp"

struct Image {
    static constexpr const int kSmallWidth     =  24;
    static constexpr const int kSmallHeight    =  32;
    static constexpr const int kMediumWidth    =  64;
    static constexpr const int kMediumHeight   =  48;
    static constexpr const int kLargeWidth     = 128;
    static constexpr const int kLargeHeight    =  96;

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
    Image                   db_image(Document, bool *wasCreated=nullptr);

    Document                document(Image);

    bool                    exists(Image);
    bool                    exists_image(uint64_t);

        //! Returns an image from a document, if it already exists in the cache
    Image                   image(Document);

        //! Returns an image from a document, if it already exists in the cache
    Image                   image(uint64_t);

    bool                    raster(ContentType);
    bool                    raster(Image);

    ContentType             type(Image);
    void                    update(Image);
}

