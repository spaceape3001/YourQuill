////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/preamble.hpp>
#include <filesystem>

#define TBL_ROOTS       "Roots"

namespace yq::mithril::cdb {

    /*! \brief All directories for root_dir
    */
    DirectoryVector             all_directories(const RootDir*, Sorted sorted=Sorted());
    
    DirectoryVector             all_directories(Root, Sorted sorted=Sorted());

    //! \brief Count of directories under root_dir
    size_t                     all_directories_count(const RootDir*);

    //! \brief All fragments for root_dir
    FragmentVector              all_fragments(const RootDir*,Sorted sorted=Sorted());
    
    //! \brief Count of all fragments for root_dir.
    size_t                     all_fragments_count(const RootDir*);
    
    //! Number of root_dirs
    size_t                     all_roots_count();

    std::vector<Root>           all_roots(Sorted sorted=Sorted{});

    //! Immediate children of root_dir (no subs)
    std::vector<DirOrFrag>     children(const RootDir*,Sorted sorted=Sorted());

    //! Create root_dir directory
    Directory                  db_root(const RootDir*, bool *wasCreated=nullptr);

    //! Top Directory for root_dir
    Directory                  directory(const RootDir*);

    //! Top Directory for root_dir
    Directory                  directory(Root);

    //Directory                  directory(const RootDir*, std::string_view);
    
    //! Top directories for root_dir (should be one)
    DirectoryVector             directories(const RootDir*, Sorted sorted=Sorted());
    
    //! Count of top directories for root_dir (should be one)
    size_t                     directories_count(const RootDir*);

    //! True if the relative file exists
    bool                       exists(const RootDir*, std::string_view );

    //! Fragment for resolving root_dir
    Fragment                   fragment(const RootDir*, std::string_view );
    
    //! Child fragments for root_dir top directory
    FragmentVector              fragments(const RootDir*, Sorted sorted=Sorted());
    
    Image                       icon(Root);
    
    bool                        is_template(Root);

    //! Key of root_dir (already on structure)
    std::string                 key(const RootDir*);
    
    //! Key of root
    std::string                 key(Root);

    std::string                 name(Root);
    
    std::filesystem::path       path(Root);

    //! Resolves the path, optionally creating the directories
    std::filesystem::path       path(const RootDir*, std::string_view , bool fMakePath=false);
    
    //! Resolves the path, optionally creating the directories
    std::filesystem::path       path(const RootDir*, Document, bool fMakePath=false);          // goes to root_dir

}
