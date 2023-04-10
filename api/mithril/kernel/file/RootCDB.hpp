////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/kernel/preamble.hpp>
#include <filesystem>

namespace yq::mithril::cdb {

    /*! \brief All directories for root
    */
    std::vector<Directory>     all_directories(const Root*, Sorted sorted=Sorted());
    
    //! \brief Count of directories under root
    size_t                     all_directories_count(const Root*);

    //! \brief All fragments for root
    std::vector<Fragment>      all_fragments(const Root*,Sorted sorted=Sorted());
    
    //! \brief Count of all fragments for root.
    size_t                     all_fragments_count(const Root*);
    
    //! Number of roots
    size_t                     all_roots_count();

    //! Immediate children of root (no subs)
    std::vector<DirOrFrag>     children(const Root*,Sorted sorted=Sorted());

    //! Create root directory
    Directory                  db_root(const Root*, bool *wasCreated=nullptr);

    //! Top Directory for root
    Directory                  directory(const Root*);
    //Directory                  directory(const Root*, std::string_view);
    
    //! Top directories for root (should be one)
    std::vector<Directory>     directories(const Root*, Sorted sorted=Sorted());
    
    //! Count of top directories for root (should be one)
    size_t                     directories_count(const Root*);

    //! True if the relative file exists
    bool                       exists(const Root*, std::string_view );

    //! Fragment for resolving root
    Fragment                   fragment(const Root*, std::string_view );
    
    //! Child fragments for root top directory
    std::vector<Fragment>      fragments(const Root*, Sorted sorted=Sorted());

    //! Key of root (already on structure)
    std::string                key(const Root*);

    //! Resolves the path, optionally creating the directories
    std::filesystem::path      path(const Root*, std::string_view , bool fMakePath=false);

    //! Resolves the path, optionally creating the directories
    std::filesystem::path      path(const Root*, Document, bool fMakePath=false);          // goes to root

}
