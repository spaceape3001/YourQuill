////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>
#include <mithril/novel/Novel.hpp>
#include <mithril/novel/NovelInfo.hpp>
#include <vector>

#define TBL_NOVELS    "Novels"
#define TBL_NOVEL_TAG "NovelTag"

namespace yq::mithril::cdb {


    /*! \brief All novels
        \param[in] sorted   Set to YES to have result sorted by key
    */
    std::vector<Novel>           all_novels(Sorted sorted=Sorted());
    
    //! \brief  Count of novels
    size_t                      all_novels_count();
    
    /*! \brief Used to create/get a novel ID from a fragment
    
        \param[in]  doc             Fragment
        \param[out] wasCreated      Pointer to boolean for true/false of creation
    */
    Novel                        db_novel(Fragment frag, bool*wasCreated=nullptr);


    /*! \brief Document for novel
    */
    Document                    document(Novel);
    
    
    //! \brief Tests for existence of novel
    bool                        exists(Novel);

    //! \brief Tests for existence of novel
    bool                        exists_novel(uint64_t);
    
    Fragment                    fragment(Novel);
    
    //! \brief Icon for novel
    Image                       icon(Novel);
    
    //! \brief One stop info for novel
    //! \param[in] autoKeyToName    If true, an empty name will be replaced by the key.
    Novel::Info                  info(Novel, bool autoKeyToName=false);
    
    //! Key for the novel
    std::string                 key(Novel);
    
    //! Label for the novel
    std::string                 label(Novel);
    
    /*! \brief ID to novel
     *      
     *  Verifies the ID is valid, and converts it to a novel
     */
    Novel                        novel(uint64_t);
    
    //! \brief Looking up a novel by string (case insensitive)
    Novel                        novel(std::string_view);

    
    //! \brief Novel for a document
    Novel                        novel(Document);

    //! \brief Novel for a fragment
    Novel                        novel(Fragment);


    //! \brief Novel by a title
    Novel                        novel_by_title(std::string_view);

    
    //! Title for the novel
    std::string                 name(Novel);

    /*! \brief Name/Key/Icon for novel
    
        This returns the Name, Key, & Icon for a novel, using a single SQL statement.
        \param[in] autoKeyToName    If true, an empty name will be replaced by the key.
    */
    NKI                         nki(Novel, bool autoKeyToName=false);
    
    //! Tests if given novel has the given tag
    bool                        tagged(Novel, Tag);
    
    //! All tags for novel
    TagVector            tags(Novel);

    //! All tags for novel as a set
    TagSet               tags_set(Novel);

    //! Title for the novel
    std::string                 title(Novel);

}
