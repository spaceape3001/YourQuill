////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>
#include <mithril/enum/SizeDesc.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/image/ImageInfo.hpp>
#include <yq/net/ContentType.hpp>
#include <yq/shape/Size2.hpp>

#define TBL_IMAGES "Images"

namespace yq::mithril::cdb {
        //! Returns a vector of all images in the cache
    std::vector<Image>      all_images(Sorted sorted=Sorted{});
    
        //! Returns a count of the available images
    size_t                  all_images_count();

    /*! \brief Finds image that best corresponds to the document
    
        This does so by replacing any final extension with the various image extensions & checking.
        
        \param[in] doc  Input document
        \return Image if successful, null otherwise
    */
    Image                   best_image(Document doc);

        //! Returns the image bytes (either from file or database)
    ByteArray               bytes(Image, SizeDesc sz=SizeDesc::Original);

        //! Creates/Retrieves an image record in the cache database
    Image                   db_image(Fragment, bool *wasCreated=nullptr);

        //! Document for image
    Document                document(Image);

        //! TRUE if the image exists (in cache database)
    bool                    exists(Image);

        //! TRUE if the image exists (in cache database)
    bool                    exists_image(uint64_t);
    
        //! Fragment for image
    Fragment                fragment(Image);

        //! Icon for root_dir
    Image                   icon(const RootDir*);

        //! For meta-generated code... pretty close to a NOP
    Image                   icon(Image);

        //! Returns an image from a document, if it already exists in the cache
    Image                   image(Document);
    
        //! Image from fragment
    Image                   image(Fragment);

        //! Image from key
    Image                   image(std::string_view);

        //! Returns an image from a document, if it already exists in the cache
    Image                   image(uint64_t);
    
    Image::Info             info(Image);

        //! TRUE if it's a raster type
    bool                    is_raster(ContentType);

        //! TRUE if it's a raster type
    bool                    is_raster(Image);

    std::string             key(Image);

        //! Mime type for image
    ContentType             mime_type(Image);
    
    //! Last Modified time code
    uint64_t                modified(Image);

    std::string             name(Image);
    
        //! Thumnail for image
    Thumbnail               thumbnail(Image, SizeDesc sz=SizeDesc::Small);

        //! Thumnail for image
    Thumbnail               thumbnail(Image, std::string_view alt, SizeDesc sz=SizeDesc::Small);

}
