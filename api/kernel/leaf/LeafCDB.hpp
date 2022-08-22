////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/atom/Atom.hpp>
#include <kernel/db/CacheFwd.hpp>
#include <kernel/enum/Sorted.hpp>
#include <kernel/file/Document.hpp>
#include <kernel/image/Image.hpp>
#include <kernel/leaf/Leaf.hpp>
#include <vector>

namespace yq {

    struct Leaf::Info {
        Atom            atom;       
        Document        doc;
        std::string     key;
        std::string     title;
        std::string     brief;
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

        Document                    document(Leaf);
        
        //! Removes this leaf from the database
        void                        erase(Leaf);
        
        bool                        exists(Leaf);
        bool                        exists_leaf(uint64_t);
        Image                       icon(Leaf);
        Leaf::Info                  info(Leaf, bool autoKeyToName=false);
        bool                        is_leaf(Atom);
        
        std::string                 key(Leaf);
        std::string                 label(Leaf);
        
        Leaf                        leaf(uint64_t);
        Leaf                        leaf(std::string_view);

        Leaf                        leaf(Atom);
        Leaf                        leaf(Document, bool calc=false);
        //Leaf                        leaf(Folder);

        Leaf                        leaf_by_title(std::string_view);

        Leaf::SharedFile            leaf_doc(Fragment, unsigned int options=0);
        
        Leaf::SharedData            merged(Leaf, cdb_options_t opts=0);
        NKI                         nki(Leaf, bool autoKeyToName=false);
        //! \brief Returns the FIRST leaf fragment that qualifies
        //! 
        //!     Will not create a fragment, though (so can return NULL)
        Leaf::SharedFile            read(Leaf, const Root*, cdb_options_t opts=0);
        std::vector<LeafFragDoc>    reads(Leaf, cdb_options_t opts=0);
        std::vector<LeafFragDoc>    reads(Leaf, class Root*, cdb_options_t opts=0);

        bool                        tagged(Leaf, Tag);
        std::vector<Tag>            tags(Leaf);
        std::set<Tag>               tags_set(Leaf);

        std::string                 title(Leaf);

        Leaf::SharedData            update(Leaf, cdb_options_t opts=0);
        void                        update_icon(Leaf);

        //!  \brief   Returns a writable document
        //!
        //!     \note the path will be CREATED by this method.
        //!
        //!     If the document already exists, it will be read in.
        Leaf::SharedFile            write(Leaf, const Root*, cdb_options_t opts=0);

    }
}
