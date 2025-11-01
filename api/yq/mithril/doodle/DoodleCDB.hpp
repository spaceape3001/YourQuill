////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/preamble.hpp>
#include <yq/mithril/atom/Atom.hpp>
#include <yq/mithril/document/Document.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/doodle/Doodle.hpp>
#include <vector>

#define TBL_DOODLES     "Doodles"
#define TBL_DOODLE_TAG  "DoodleTag"

namespace yq::mithril::cdb {


    /*! \brief All doodles
        \param[in] sorted   Set to YES to have result sorted by key
    */
    DoodleVector             all_doodles(Sorted sorted=Sorted());
    
    //! \brief  Count of doodles
    size_t                      all_doodles_count();
    

    /*! \brief Used to create/get a doodle ID from a fragment
    
        \param[in]  frag            Fragment
        \param[out] wasCreated      Pointer to boolean for true/false of creation
    */
    Doodle                        db_doodle(Fragment frag, bool*wasCreated=nullptr);


    /*! \brief Document for doodle
    */
    Document                    document(Doodle);
    
    /*! \brief ID to doodle
     *      
     *  Verifies the ID is valid, and converts it to a doodle
     */
    Doodle                        doodle(uint64_t);
    
    //! \brief Looking up a doodle by string (case insensitive)
    Doodle                        doodle(std::string_view);

    //! \brief First doodle for a document
    Doodle                        doodle(Document);

    //! Doodle for a fragment
    Doodle                        doodle(Fragment, bool calc=false);
    
    //! \brief Tests for existence of doodle
    bool                        exists(Doodle);

    //! \brief Tests for existence of doodle
    bool                        exists_doodle(uint64_t);
    
    //! Fragment for a doodle
    Fragment                    fragment(Doodle);
    
    Image                       icon(Doodle);
    
    //! \brief One stop info for doodle
    //! \param[in] autoKeyToName    If true, an empty name will be replaced by the key.
    Doodle::Info                info(Doodle, bool autoKeyToName=false);
    
    //! Key for the doodle
    std::string                 key(Doodle);
    
    //! Label for the doodle
    std::string                 label(Doodle);
    
    //! Title for the doodle
    std::string                 name(Doodle);

    /*! \brief Name/Key/Icon for doodle
    
        This returns the Name, Key, & Icon for a doodle, using a single SQL statement.
        \param[in] autoKeyToName    If true, an empty name will be replaced by the key.
    */
    NKI                         nki(Doodle, bool autoKeyToName=false);

   //! Tests if given doodle has the given tag
    bool                        tagged(Doodle, Tag);
    
    //! All tags for doodle
    TagVector                   tags(Doodle);

    //! All tags for doodle as a set
    TagSet                      tags_set(Doodle);

    //! Title for the doodle
    std::string                 title(Doodle);
}
