////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/preamble.hpp>
#include <kernel/enum/DataRole.hpp>
#include <kernel/file/Document.hpp>
#include <kernel/file/Folder.hpp>
#include <kernel/image/Image.hpp>
#include <basic/Http.hpp>

namespace yq {

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
        std::vector<Document>           all_documents(Sorted sorted=Sorted());
        size_t                          all_documents_count();
        std::vector<Document>           all_documents_suffix(std::string_view , Sorted sorted=Sorted());
        
        std::vector<Document>           all_similar(Document, Sorted=Sorted{});
        std::vector<Document>           all_similar(Document, const std::vector<Extension>&, Sorted=Sorted{});
        std::vector<Document>           all_similar(Document, const std::vector<ExtensionView>&, Sorted=Sorted{});

        std::string                     base_key(Document);         //!< key w/o final extensions

        
    #if 0
        std::string                     child_key(Document);
    #endif

        Document                        db_document(Folder, std::string_view , bool *wasCreated=nullptr);

        Document                        document(std::string_view );
        Document                        document(uint64_t);
        
        bool                            exists(Document);
        /*! \brief Document existence with Root
        
            Checks to see if the document has a fragment with the specified root.  
            Will return FALSE if root is null.
        */
        bool                            exists(Document, const Root*);
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
        std::vector<Fragment>           fragments(Document, const Root*, Sorted sorted=Sorted());
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

        /*! \brief Roots for document
        
            All the roots that has at least one fragment for the given document.
        */
        std::vector<const Root*>        roots(Document);

        size_t                          roots_count(Document);

        void                            set_mime_type(Document, ContentType);
        
        Document                        similar(Document, const ExtensionView&);
        Document                        similar(Document, std::string_view newMiddle, const ExtensionView&);
        
        void                            show(Document);

        std::string                     skey(Document);   //!< Key inside the directory

        std::string                     skeyb(Document);  //!< Key inside the directory (w/o any extensions)

        std::string                     skeyc(Document);  //!< Key inside the directory (w/o final extension)

        Extension                       suffix(Document);

        Fragment                        writable(Document, DataRole);
    }
}
