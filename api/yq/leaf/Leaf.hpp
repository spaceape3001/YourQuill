////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "LeafFile.hpp"

#include "yq/bit/Context.hpp"
#include "yq/file/FileSys.hpp"
#include "yq/util/Map.hpp"

class LeafFile;

struct Leaf {
    static constexpr const char*    szExtension = "y";
    struct Info;
    struct Merge;
    using FragDoc = std::pair<Fragment,LeafFile::Shared>;
    uint64_t  id  = 0ULL;
    constexpr auto    operator<=>(const Leaf&rhs) const = default;
    constexpr operator bool() const { return id != 0ULL; }
};

struct Leaf::Info {
    //  Atom        atom;   circular dependencies
    Document    doc;
    QString     key;
    QString     title;
    auto operator<=>(const Info&) const = default;
};


struct Leaf::Merge : public LeafFile {
    using Shared    = std::shared_ptr<Merge>;
    Map<const Root*,Context>    context;
};

namespace cdb {


    Vector<Atom>            all_leaf_atoms(Sorted sorted=Sorted());
    size_t                  all_leaf_atoms_count();

    Vector<Leaf>            all_leafs(Sorted sorted=Sorted());
    size_t                  all_leafs_count();
    Atom                    atom(Leaf);

    Leaf                    db_leaf(Document, bool*wasCreated=nullptr);

    Document                document(Leaf);
    bool                    exists(Leaf);
    bool                    exists_leaf(uint64_t);
    Folder                  folder(Leaf);
    Image                   icon(Leaf);
    Leaf::Info              info(Leaf, bool autoKeyToName=false);
    bool                    is_leaf(Atom);
    
    QString                 key(Leaf);
    QString                 label(Leaf);
    
    Leaf                    leaf(uint64_t);
    Leaf                    leaf(const QString&);

    Leaf                    leaf(Atom);
    Leaf                    leaf(Document);
    Leaf                    leaf(Folder);

    LeafFile::Shared        leaf_doc(Fragment, bool fAllowEmpty=false);
    
    Leaf::Merge::Shared     merged(Leaf, unsigned int opts=0);
    NKI                     nki(Leaf, bool autoKeyToName=false);
    //! \brief Returns the FIRST leaf fragment that qualifies
    //! 
    //!     Will not create a fragment, though (so can return NULL)
    LeafFile::Shared        read(Leaf, const Root*);
    Vector<Leaf::FragDoc>   reads(Leaf);
    Vector<Leaf::FragDoc>   reads(Leaf, class Root*);

    QString                 title(Leaf);

    //!  \brief   Returns a writable document
    //!
    //!     \note the path will be CREATED by this method.
    //!
    //!     If the document already exists, it will be read in.
    LeafFile::Shared         write(Leaf, const Root*);

}
