////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/core/cdb_common.hpp>
#include <db/atomsys/atom.hpp>
#include <db/docsys/leaf.hpp>
#include <db/enum/sorted.hpp>
#include <db/filesys/document.hpp>
#include <db/imagesys/image.hpp>
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

        std::vector<Atom>           all_leaf_atoms(Sorted sorted=Sorted());
        size_t                      all_leaf_atoms_count();

        std::vector<Leaf>           all_leafs(Sorted sorted=Sorted());
        size_t                      all_leafs_count();
        Atom                        atom(Leaf);
        
        std::string                 brief(Leaf);

        Leaf                        db_leaf(Document, bool*wasCreated=nullptr);

        //Folder                    detail_folder(Leaf);

        Document                    document(Leaf);
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
        Leaf                        leaf(Document);
        //Leaf                        leaf(Folder);

        Leaf                        leaf_by_title(std::string_view);

        Leaf::SharedFile            leaf_doc(Fragment, unsigned int options=0);
        
        Leaf::SharedData            merged(Leaf, unsigned int opts=0);
        NKI                         nki(Leaf, bool autoKeyToName=false);
        //! \brief Returns the FIRST leaf fragment that qualifies
        //! 
        //!     Will not create a fragment, though (so can return NULL)
        Leaf::SharedFile            read(Leaf, const Root*, unsigned int opts=0);
        std::vector<LeafFragDoc>    reads(Leaf, unsigned int opts=0);
        std::vector<LeafFragDoc>    reads(Leaf, class Root*, unsigned int opts=0);

        std::string                 title(Leaf);

        //!  \brief   Returns a writable document
        //!
        //!     \note the path will be CREATED by this method.
        //!
        //!     If the document already exists, it will be read in.
        Leaf::SharedFile            write(Leaf, const Root*, unsigned int opts=0);

    }
}