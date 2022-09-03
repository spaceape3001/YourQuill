////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/db/CacheFwd.hpp>
#include <kernel/enum/SizeDesc.hpp>
#include <kernel/image/Image.hpp>
#include <basic/ContentType.hpp>


namespace yq {
    struct Thumbnail;
    class ByteArray;
    
    namespace cdb {
            //! Returns a vector of all images in the cache
        std::vector<Image>      all_images();
        
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

            //! Removes this image from the cache
        void                    erase(Image);

            //! TRUE if the image exists (in cache database)
        bool                    exists(Image);

            //! TRUE if the image exists (in cache database)
        bool                    exists_image(uint64_t);
        
            //! Fragment for image
        Fragment                fragment(Image);

            //! Icon for root
        Image                   icon(const Root*);

            //! Returns an image from a document, if it already exists in the cache
        Image                   image(Document);
        
            //! Image from fragment
        Image                   image(Fragment);

            //! Image from key
        Image                   image(std::string_view);

            //! Returns an image from a document, if it already exists in the cache
        Image                   image(uint64_t);

            //! TRUE if it's a raster type
        bool                    is_raster(ContentType);

            //! TRUE if it's a raster type
        bool                    is_raster(Image);

            //! Mime type for image
        ContentType             mime_type(Image);
        
        //std::vector<Image>
        
            //! Thumnail for image
        Thumbnail               thumbnail(Image, SizeDesc sz=SizeDesc::Small);

            //! Thumnail for image
        Thumbnail               thumbnail(Image, std::string_view alt, SizeDesc sz=SizeDesc::Small);

    }
}
