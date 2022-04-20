////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "cdb_common.hpp"
#include "directory.hpp"
#include "enum_data_role.hpp"
#include "enum_sorted.hpp"
#include "folder.hpp"
#include "image.hpp"


namespace yq {

    struct Directory::Info {
        Folder                  folder;
        bool                    hidden      = false;
        std::string             name;
        Directory               parent;
        std::filesystem::path   path;
        bool                    removed     = false;
        const Root*             root        = nullptr;
        
        bool operator==(const Info&) const = default;
    };

    namespace cdb {

        /*! \brief Returns all directories in the cache
        
            \param[in]  opts  Options to search for
            \return Vector of directories
        */
        Vector<Directory>  all_directories(unsigned opts=0);

        /*! \brief Returns all directories in the cache
        
            \param[in]  sorted  Yes/No to sort by path (default is no)
            \return Vector of directories
        */
        Vector<Directory>  all_directories(Sorted sorted);
        
        //! \brief Full count of directories in the search
        size_t  all_directories_count();
        
        /*! \brief Child of directory
        
            \param[in] par  Parent directory;
            \param[in] k    Child key
        */
        DirOrFrag  child(Directory par, std::string_view k);

        /*! \brief Total number of children in directory
            \param[in] par  Parent directory;
        */
        size_t  child_count(Directory par);

        /*! \brief Sub-directories of the directory
            \param[in] par      Parent directory;
            \param[in] sorted   Flag to sort by name
        */
        Vector<Directory>  child_directories(Directory par, Sorted sorted=Sorted());
        
        /*! \brief Number of sub-directories of the directory
            \param[in] par      Parent directory;
        */
        size_t  child_directories_count(Directory);

        /*! \brief Sub-directories of the directory (as a set)
            \param[in] par      Parent directory;
        */
        Set<Directory>  child_directories_set(Directory par);

        Vector<DirString>          child_directories_with_names(Directory, Sorted sorted=Sorted());

        Directory                       child_directory(Directory, std::string_view );
        Fragment                        child_fragment(Directory, std::string_view );
        Vector<Fragment>           child_fragments(Directory, Sorted sorted=Sorted());
        size_t                          child_fragments_count(Directory);
        Set<Fragment>              child_fragments_set(Directory);



    #if 0
        std::string                     child_key(Directory);       //<! Key inside the directory (with extensions)
    #endif

        Vector<DirOrFrag>          children(Directory,Sorted sorted=Sorted());

        Vector<DirOrFragStr>       children_and_names(Directory);


        //Vector<uint8_t>         data(Fragment);   // TODO

        std::pair<Directory, Folder>    db_directory(Directory, std::string_view , bool *wasCreated=nullptr);

        Directory                       directory(uint64_t);
        Directory                       directory(const std::filesystem::path&);
        
            //Vector<Document>      documents(Directory);   // TODO

        void                            erase(Directory);

        bool                            exists(Directory);

        bool                            exists_directory(uint64_t);


        Folder                          folder(Directory);
        
        Vector<Folder>             folder_path(Directory);


        bool                            hidden(Directory);
        Directory::Info                 info(Directory);

        std::string                     key(Directory);

        //std::string                 label(Directory);

        std::string                     name(Directory);
        Directory                       parent(Directory);

        std::filesystem::path           path(Directory);

        bool                            removed(Directory);
        const Root*                     root(Directory);
        std::string                     skey(Directory);  //!< Key inside the directory
        std::string                     skeyb(Directory); //!< Key inside the directory (w/o extensions)
    }
}
