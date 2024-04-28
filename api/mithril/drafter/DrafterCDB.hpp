////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>
#include <mithril/atom/Atom.hpp>
#include <mithril/document/Document.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/drafter/Drafter.hpp>
#include <vector>

#define TBL_DRAFTERS   "Drafters"

namespace yq::mithril::cdb {

    using DrafterFragDoc = std::pair<Fragment,Drafter::SharedFile>;


    /*! \brief All drafters
        \param[in] sorted   Set to YES to have result sorted by key
    */
    DrafterVector             all_drafters(Sorted sorted=Sorted());
    
    //! \brief  Count of drafters
    size_t                      all_drafters_count();
    
    //! \brief Brief description of the drafter
    std::string                 brief(Drafter);

    /*! \brief Used to create/get a drafter ID from a document 
    
        \param[in]  doc             Document 
        \param[out] wasCreated      Pointer to boolean for true/false of creation
    */
    Drafter                        db_drafter(Document doc, bool*wasCreated=nullptr);

    //Folder                    detail_folder(Drafter);


    /*! \brief Document for drafter
    */
    Document                    document(Drafter);
    
    
    //! \brief Tests for existence of drafter
    bool                        exists(Drafter);

    //! \brief Tests for existence of drafter
    bool                        exists_drafter(uint64_t);
    
    //! \brief Icon for drafter
    Image                       icon(Drafter);
    
    //! \brief One stop info for drafter
    //! \param[in] autoKeyToName    If true, an empty name will be replaced by the key.
    Drafter::Info                 info(Drafter, bool autoKeyToName=false);
    
    //! Key for the drafter
    std::string                 key(Drafter);
    
    //! Label for the drafter
    std::string                 label(Drafter);
    
    
    
    /*! \brief Merged Drafter structure
     *  
     *  This FUSES all fragments together for a single, cohesive view of the drafter.
    */
    Drafter::SharedData            merged(Drafter, cdb_options_t opts=0);
    
    //! Title for the drafter
    std::string                 name(Drafter);

    /*! \brief Name/Key/Icon for drafter
    
        This returns the Name, Key, & Icon for a drafter, using a single SQL statement.
        \param[in] autoKeyToName    If true, an empty name will be replaced by the key.
    */
    NKI                         nki(Drafter, bool autoKeyToName=false);
    
    //! \brief Returns the FIRST drafter fragment that qualifies
    //! 
    //!     Will not create a fragment, though (so can return NULL)
    Drafter::SharedFile            read(Drafter, const RootDir*, cdb_options_t opts=0);
    
    /*! \brief Returns ALL drafter fragments
    
        This returns a vector of ALL drafter fragments for drafter.  (No merging)
    */
    std::vector<DrafterFragDoc>    reads(Drafter, cdb_options_t opts=0);

    /*! \brief Returns drafter fragments for given root_dir
    
        This returns a vector of fragments for given drafter.  (No merging)
        
        \note this will only be one, at most on a case-insensitive file system.  However, on a case sensitive
        file system, this may be multiple.
    */
    std::vector<DrafterFragDoc>    reads(Drafter, class RootDir*, cdb_options_t opts=0);

    /*! \brief ID to drafter
     *      
     *  Verifies the ID is valid, and converts it to a drafter
     */
    Drafter                        drafter(uint64_t);
    
    //! \brief Looking up a drafter by string (case insensitive)
    Drafter                        drafter(std::string_view);

    //! \brief Document for an atom
    Drafter                        drafter(Document, bool calc=false);
    //Drafter                        drafter(Folder);

    /*! \brief Drafter document
     *  
     *  This loads a SINGLE fragment into a drafter structure.
     *  \note This may be incomplete as there can be multiple fragments to a drafter.
     */
    Drafter::SharedFile            drafter_doc(Fragment, cdb_options_t options={});


    //!  \brief   Returns a writable document
    //!
    //!     \note the path will be CREATED by this method.
    //!
    //!     If the document already exists, it will be read in.
    Drafter::SharedFile            write(Drafter, const RootDir*, cdb_options_t opts={});

}
