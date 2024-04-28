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
#include <mithril/doodle/Doodle.hpp>
#include <vector>

#define TBL_DOODLES   "Doodles"

namespace yq::mithril::cdb {

    using DoodleFragDoc = std::pair<Fragment,Doodle::SharedFile>;


    /*! \brief All doodles
        \param[in] sorted   Set to YES to have result sorted by key
    */
    DoodleVector             all_doodles(Sorted sorted=Sorted());
    
    //! \brief  Count of doodles
    size_t                      all_doodles_count();
    
    //! \brief Brief description of the doodle
    std::string                 brief(Doodle);

    /*! \brief Used to create/get a doodle ID from a document 
    
        \param[in]  doc             Document 
        \param[out] wasCreated      Pointer to boolean for true/false of creation
    */
    Doodle                        db_doodle(Document doc, bool*wasCreated=nullptr);

    //Folder                    detail_folder(Doodle);


    /*! \brief Document for doodle
    */
    Document                    document(Doodle);
    
    
    //! \brief Tests for existence of doodle
    bool                        exists(Doodle);

    //! \brief Tests for existence of doodle
    bool                        exists_doodle(uint64_t);
    
    //! \brief Icon for doodle
    Image                       icon(Doodle);
    
    //! \brief One stop info for doodle
    //! \param[in] autoKeyToName    If true, an empty name will be replaced by the key.
    Doodle::Info                 info(Doodle, bool autoKeyToName=false);
    
    //! Key for the doodle
    std::string                 key(Doodle);
    
    //! Label for the doodle
    std::string                 label(Doodle);
    
    
    
    /*! \brief Merged Doodle structure
     *  
     *  This FUSES all fragments together for a single, cohesive view of the doodle.
    */
    Doodle::SharedData            merged(Doodle, cdb_options_t opts=0);
    
    //! Title for the doodle
    std::string                 name(Doodle);

    /*! \brief Name/Key/Icon for doodle
    
        This returns the Name, Key, & Icon for a doodle, using a single SQL statement.
        \param[in] autoKeyToName    If true, an empty name will be replaced by the key.
    */
    NKI                         nki(Doodle, bool autoKeyToName=false);
    
    //! \brief Returns the FIRST doodle fragment that qualifies
    //! 
    //!     Will not create a fragment, though (so can return NULL)
    Doodle::SharedFile            read(Doodle, const RootDir*, cdb_options_t opts=0);
    
    /*! \brief Returns ALL doodle fragments
    
        This returns a vector of ALL doodle fragments for doodle.  (No merging)
    */
    std::vector<DoodleFragDoc>    reads(Doodle, cdb_options_t opts=0);

    /*! \brief Returns doodle fragments for given root_dir
    
        This returns a vector of fragments for given doodle.  (No merging)
        
        \note this will only be one, at most on a case-insensitive file system.  However, on a case sensitive
        file system, this may be multiple.
    */
    std::vector<DoodleFragDoc>    reads(Doodle, class RootDir*, cdb_options_t opts=0);

    /*! \brief ID to doodle
     *      
     *  Verifies the ID is valid, and converts it to a doodle
     */
    Doodle                        doodle(uint64_t);
    
    //! \brief Looking up a doodle by string (case insensitive)
    Doodle                        doodle(std::string_view);

    //! \brief Document for an atom
    Doodle                        doodle(Document, bool calc=false);
    //Doodle                        doodle(Folder);

    /*! \brief Doodle document
     *  
     *  This loads a SINGLE fragment into a doodle structure.
     *  \note This may be incomplete as there can be multiple fragments to a doodle.
     */
    Doodle::SharedFile            doodle_doc(Fragment, cdb_options_t options={});


    //!  \brief   Returns a writable document
    //!
    //!     \note the path will be CREATED by this method.
    //!
    //!     If the document already exists, it will be read in.
    Doodle::SharedFile            write(Doodle, const RootDir*, cdb_options_t opts={});

}
