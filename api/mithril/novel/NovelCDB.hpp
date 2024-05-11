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
#include <mithril/novel/Novel.hpp>
#include <vector>

#define TBL_NOVELS    "Novels"
#define TBL_NOVEL_TAG "NovelTag"

namespace yq::mithril {

    struct Novel::Info {
        //Atom            atom;       
        Document        doc;
        std::string     key;
        std::string     title;
        //std::string     brief;
        //std::string     abbr;
        Image           icon;
        bool operator==(const Info&) const = default;
    };

    namespace cdb {

        using NovelFragDoc = std::pair<Fragment,Novel::SharedFile>;

        /*! \brief All atoms that are novels
        
            This returns all atoms that are also associated with novels.
            \param[in] sorted   Set to YES to have result sorted by key
        */
        //AtomVector           all_novel_atoms(Sorted sorted=Sorted());
        
        /*! \brief Count of atoms that are the primary from a novel
        */
        //size_t                      all_novel_atoms_count();

        /*! \brief All novels
            \param[in] sorted   Set to YES to have result sorted by key
        */
        std::vector<Novel>           all_novels(Sorted sorted=Sorted());
        
        //! \brief  Count of novels
        size_t                      all_novels_count();
        
        //! \brief Primary atom for the novel
        //Atom                        atom(Novel);
        
        //! \brief Brief description of the novel
        //std::string                 brief(Novel);

        /*! \brief Used to create/get a novel ID from a document 
        
            \param[in]  doc             Document 
            \param[out] wasCreated      Pointer to boolean for true/false of creation
        */
        Novel                        db_novel(Document doc, bool*wasCreated=nullptr);

        //Folder                    detail_folder(Novel);


        /*! \brief Document for novel
        */
        Document                    document(Novel);
        
        
        //! \brief Tests for existence of novel
        bool                        exists(Novel);

        //! \brief Tests for existence of novel
        bool                        exists_novel(uint64_t);
        
        //! \brief Icon for novel
        Image                       icon(Novel);
        
        //! \brief One stop info for novel
        //! \param[in] autoKeyToName    If true, an empty name will be replaced by the key.
        Novel::Info                  info(Novel, bool autoKeyToName=false);
        
        //! Detects if atom is also primary for novel
        //bool                        is_novel(Atom);
        
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

        //! \brief Novel of an atom
        //Novel                        novel(Atom);
        
        //! \brief Document for an atom
        Novel                        novel(Document, bool calc=false);
        //Novel                        novel(Folder);

        //! \brief Novel by a title
        Novel                        novel_by_title(std::string_view);

        /*! \brief Novel document
         *  
         *  This loads a SINGLE fragment into a novel structure.
         *  \note This may be incomplete as there can be multiple fragments to a novel.
         */
        Novel::SharedFile            novel_doc(Fragment, unsigned int options=0);
        
        /*! \brief Merged Novel structure
         *  
         *  This FUSES all fragments together for a single, cohesive view of the novel.
        */
        Novel::SharedData            merged(Novel, cdb_options_t opts=0);
        
        //! Title for the novel
        std::string                 name(Novel);

        /*! \brief Name/Key/Icon for novel
        
            This returns the Name, Key, & Icon for a novel, using a single SQL statement.
            \param[in] autoKeyToName    If true, an empty name will be replaced by the key.
        */
        NKI                         nki(Novel, bool autoKeyToName=false);
        
        //! \brief Returns the FIRST novel fragment that qualifies
        //! 
        //!     Will not create a fragment, though (so can return NULL)
        Novel::SharedFile            read(Novel, const RootDir*, cdb_options_t opts=0);
        
        /*! \brief Returns ALL novel fragments
        
            This returns a vector of ALL novel fragments for novel.  (No merging)
        */
        std::vector<NovelFragDoc>    reads(Novel, cdb_options_t opts=0);

        /*! \brief Returns novel fragments for given root_dir
        
            This returns a vector of fragments for given novel.  (No merging)
            
            \note this will only be one, at most on a case-insensitive file system.  However, on a case sensitive
            file system, this may be multiple.
        */
        std::vector<NovelFragDoc>    reads(Novel, class RootDir*, cdb_options_t opts=0);

        //! Tests if given novel has the given tag
        bool                        tagged(Novel, Tag);
        
        //! All tags for novel
        TagVector            tags(Novel);

        //! All tags for novel as a set
        TagSet               tags_set(Novel);

        //! Title for the novel
        std::string                 title(Novel);

        //!  \brief   Returns a writable document
        //!
        //!     \note the path will be CREATED by this method.
        //!
        //!     If the document already exists, it will be read in.
        Novel::SharedFile            write(Novel, const RootDir*, cdb_options_t opts=0);

    }
}
