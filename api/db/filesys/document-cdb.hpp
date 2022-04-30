////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/core/cdb_common.hpp>
#include <db/enum/data_role.hpp>
#include <db/enum/sorted.hpp>
#include <db/filesys/document.hpp>
#include <db/filesys/folder.hpp>
#include <db/imagesys/image.hpp>
#include <yq/net/Http.hpp>

namespace yq {
    struct Fragment;
    struct Root;
    class ByteArray;

    struct Document::Info {
        Folder          folder;
        bool            hidden      = false;
        std::string     key;
        std::string     name;
        bool            removed     = false;
        std::string     skey;
        std::string     skeyb;
        std::string     skeyc;
        std::string     suffix;
        Image           icon;
        
        bool operator==(const Info&) const = default;
    };

    namespace cdb {
        struct NKI;

        std::vector<Document>           all_documents(Sorted sorted=Sorted());
        size_t                     all_documents_count();
        std::vector<Document>           all_documents_suffix(std::string_view , Sorted sorted=Sorted());
        
        std::string                     base_key(Document);         //!< key w/o final extensions

        /*! \brief Finds image that best corresponds to the document
        
            This does so by replacing any final extension with the various image extensions & checking.
            
            \param[in] doc  Input document
            \return Image if successful, null otherwise
        */
        Image                           best_image(Document doc);
        
    #if 0
        std::string                     child_key(Document);
    #endif

        Document                        db_document(Folder, std::string_view , bool *wasCreated=nullptr);

        Document                        document(std::string_view );
        Document                        document(uint64_t);
        
        bool                            exists(Document);
        bool                            exists_document(uint64_t);

        /*! \brief Returns the FIRST document encountered
        */
        Document                        first_document(std::initializer_list<std::string_view>);
        Document                        first_document(const std::vector<std::string>&);

        Fragment                        first_fragment(Document);
        Fragment                        first_fragment(Document, const Root*);
        Fragment                        first_fragment(Document, DataRole);

        Folder                          folder(Document);
        
        std::vector<Folder>             folder_path(Document);
        
        /*! \brief Returns the first fragmment found for the given document
        */
        Fragment                        fragment(Document);
        Fragment                        fragment(Document, const Root*);
        Fragment                        fragment(Document, DataRole);
        
        std::vector<Fragment>           fragments(Document, Sorted sorted=Sorted());
        std::vector<Fragment>           fragments(Document, Sorted::Value sorted);
        std::vector<Fragment>           fragments(Document, const Root*, Sorted sorted=Sorted());
        std::vector<Fragment>           fragments(Document, const Root*, Sorted::Value sorted);
        std::vector<Fragment>           fragments(Document, DataRole, Sorted sorted=Sorted());

        size_t                          fragments_count(Document);
        size_t                          fragments_count(Document, const Root*);

        bool                            hidden(Document);
        
        void                            hide(Document);

        Image                           icon(Document);

        Document::Info                  info(Document);

        std::string                     key(Document);

        std::string                     label(Document);
        
        ContentType                     mime_type(Document);
        std::string                     name(Document);

        NKI                             nki(Document, bool autoKeyToName=false);

        Folder                          parent(Document);


        bool                            removed(Document);

        std::vector<const Root*>         roots(Document);

        size_t                          roots_count(Folder);

        void                            set_mime_type(Document, ContentType);

        void                            show(Document);

        std::string                     skey(Document);   //!< Key inside the directory

        std::string                     skeyb(Document);  //!< Key inside the directory (w/o any extensions)

        std::string                     skeyc(Document);  //!< Key inside the directory (w/o final extension)

        std::string                     suffix(Document);

        Fragment                        writable(Document, DataRole);
    }
}
