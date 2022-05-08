////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/db/CacheFwd.hpp>
#include <yq/enum/SizeDesc.hpp>
#include <yq/image/Image.hpp>
#include <yq/net/Http.hpp>


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

        Document                document(Image);

            //! Removes this image from the cache
        void                    erase(Image);

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

        bool                    is_raster(ContentType);
        bool                    is_raster(Image);

        ContentType             mime_type(Image);
        
        //std::vector<Image>
        
        Thumbnail               thumbnail(Image, SizeDesc sz=SizeDesc::Small);
        void                    update_root(const Root*, Image);
    }
}
