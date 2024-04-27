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
#include <mithril/leaf/Leaf.hpp>
#include <vector>

#define TBL_LEAFS    "Leafs"
#define TBL_LEAF_TAG "LeafTag"

namespace yq::mithril {

    struct Leaf::Info {
        Atom            atom;       
        Document        doc;
        std::string     key;
        std::string     title;
        std::string     brief;
        std::string     abbr;
        Image           icon;
        bool operator==(const Info&) const = default;
    };

    namespace cdb {

        using LeafFragDoc = std::pair<Fragment,Leaf::SharedFile>;

        /*! \brief All atoms that are leafs
        
            This returns all atoms that are also associated with leafs.
            \param[in] sorted   Set to YES to have result sorted by key
        */
        std::vector<Atom>           all_leaf_atoms(Sorted sorted=Sorted());
        
        /*! \brief Count of atoms that are the primary from a leaf
        */
        size_t                      all_leaf_atoms_count();

        /*! \brief All leafs
            \param[in] sorted   Set to YES to have result sorted by key
        */
        std::vector<Leaf>           all_leafs(Sorted sorted=Sorted());
        
        //! \brief  Count of leafs
        size_t                      all_leafs_count();
        
        //! \brief Primary atom for the leaf
        Atom                        atom(Leaf);
        
        //! \brief Brief description of the leaf
        std::string                 brief(Leaf);

        /*! \brief Used to create/get a leaf ID from a document 
        
            \param[in]  doc             Document 
            \param[out] wasCreated      Pointer to boolean for true/false of creation
        */
        Leaf                        db_leaf(Document doc, bool*wasCreated=nullptr);

        //Folder                    detail_folder(Leaf);


        /*! \brief Document for leaf
        */
        Document                    document(Leaf);
        
        
        //! \brief Tests for existence of leaf
        bool                        exists(Leaf);

        //! \brief Tests for existence of leaf
        bool                        exists_leaf(uint64_t);
        
        //! \brief Icon for leaf
        Image                       icon(Leaf);
        
        //! \brief One stop info for leaf
        //! \param[in] autoKeyToName    If true, an empty name will be replaced by the key.
        Leaf::Info                  info(Leaf, bool autoKeyToName=false);
        
        //! Detects if atom is also primary for leaf
        bool                        is_leaf(Atom);
        
        //! Key for the leaf
        std::string                 key(Leaf);
        
        //! Label for the leaf
        std::string                 label(Leaf);
        
        /*! \brief ID to leaf
         *      
         *  Verifies the ID is valid, and converts it to a leaf
         */
        Leaf                        leaf(uint64_t);
        
        //! \brief Looking up a leaf by string (case insensitive)
        Leaf                        leaf(std::string_view);

        //! \brief Leaf of an atom
        Leaf                        leaf(Atom);
        
        //! \brief Document for an atom
        Leaf                        leaf(Document, bool calc=false);
        //Leaf                        leaf(Folder);

        //! \brief Leaf by a title
        Leaf                        leaf_by_title(std::string_view);

        /*! \brief Leaf document
         *  
         *  This loads a SINGLE fragment into a leaf structure.
         *  \note This may be incomplete as there can be multiple fragments to a leaf.
         */
        Leaf::SharedFile            leaf_doc(Fragment, unsigned int options=0);
        
        /*! \brief Merged Leaf structure
         *  
         *  This FUSES all fragments together for a single, cohesive view of the leaf.
        */
        Leaf::SharedData            merged(Leaf, cdb_options_t opts=0);
        
        //! Title for the leaf
        std::string                 name(Leaf);

        /*! \brief Name/Key/Icon for leaf
        
            This returns the Name, Key, & Icon for a leaf, using a single SQL statement.
            \param[in] autoKeyToName    If true, an empty name will be replaced by the key.
        */
        NKI                         nki(Leaf, bool autoKeyToName=false);
        
        //! \brief Returns the FIRST leaf fragment that qualifies
        //! 
        //!     Will not create a fragment, though (so can return NULL)
        Leaf::SharedFile            read(Leaf, const RootDir*, cdb_options_t opts=0);
        
        /*! \brief Returns ALL leaf fragments
        
            This returns a vector of ALL leaf fragments for leaf.  (No merging)
        */
        std::vector<LeafFragDoc>    reads(Leaf, cdb_options_t opts=0);

        /*! \brief Returns leaf fragments for given root_dir
        
            This returns a vector of fragments for given leaf.  (No merging)
            
            \note this will only be one, at most on a case-insensitive file system.  However, on a case sensitive
            file system, this may be multiple.
        */
        std::vector<LeafFragDoc>    reads(Leaf, class RootDir*, cdb_options_t opts=0);

        //! Tests if given leaf has the given tag
        bool                        tagged(Leaf, Tag);
        
        //! All tags for leaf
        TagVector            tags(Leaf);

        //! All tags for leaf as a set
        TagSet               tags_set(Leaf);

        //! Title for the leaf
        std::string                 title(Leaf);

        //!  \brief   Returns a writable document
        //!
        //!     \note the path will be CREATED by this method.
        //!
        //!     If the document already exists, it will be read in.
        Leaf::SharedFile            write(Leaf, const RootDir*, cdb_options_t opts=0);

    }
}
