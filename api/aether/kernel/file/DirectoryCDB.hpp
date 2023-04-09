////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <aether/kernel/preamble.hpp>
#include <basic/Set.hpp>
#include <aether/kernel/file/Directory.hpp>
#include <aether/kernel/file/Folder.hpp>

#include <filesystem>
#include <vector>

namespace yq {

    /*! \brief Information fro a directory
    
        This is the fast-data return for the "info" below.
    */
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
            \return std::vector of directories
        */
        std::vector<Directory>  all_directories(unsigned opts=0);

        /*! \brief Returns all directories in the cache
        
            \param[in]  sorted  Yes/No to sort by path (default is no)
            \return std::vector of directories
        */
        std::vector<Directory>  all_directories(Sorted sorted);
        
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
        std::vector<Directory>  child_directories(Directory par, Sorted sorted=Sorted());
        
        /*! \brief Number of sub-directories of the directory
            \param[in] par      Parent directory;
        */
        size_t  child_directories_count(Directory);

        /*! \brief Sub-directories of the directory (as a set)
            \param[in] par      Parent directory;
        */
        Set<Directory>  child_directories_set(Directory par);

        /*! \brief List of child directories with names
        
            \param[in] par      Parent directory
            \param[in] sorted   Flag to sort by name
        */
        std::vector<DirString>          child_directories_with_names(Directory par, Sorted sorted=Sorted());

        /*! \brief Child directory with name
            \param[in] par      Parent directory
            \param[in] ck       Key for child directory
            \return FIRST directory that matches
        */
        Directory                       child_directory(Directory, std::string_view ck);
        
        /*! \brief Fragment with name
        
            \param[in] par      Parent directory
            \param[in] ck       Key for frment
            
            \return FIRST fragment that matches
        */
        Fragment                        child_fragment(Directory par, std::string_view ck);
        
        /*! \brief All fragments for directory
        
            \param[in] par      Parent directory
            \param[in] sorted   Flag to sort by name
            \return VECTOR of fragments
        */
        std::vector<Fragment>           child_fragments(Directory par, Sorted sorted=Sorted());
        
        /*! \brief Number of fragments for directory
            \param[in] par      Parent directory
            \return COUNT (as shown in the database)
        */
        size_t                          child_fragments_count(Directory par);
        
        /*! \brief All fragments for directory as ordered set
            \param[in] par      Parent directory
        */
        Set<Fragment>              child_fragments_set(Directory par);



    #if 0
        std::string                     child_key(Directory);       //<! Key inside the directory (with extensions)
    #endif

        std::vector<DirOrFrag>          children(Directory,Sorted sorted=Sorted());

        std::vector<DirOrFragStr>       children_and_names(Directory);


        //std::vector<uint8_t>         data(Fragment);   // TODO

        std::pair<Directory, Folder>    db_directory(Directory, std::string_view , bool *wasCreated=nullptr);

        /*! \brief Gets directory by ID
        
            This verifies the directory by ID and returns it.
            
            \param[in] id   The ID of the directory
            
            \return Directory if valid, zero if not.
        */
        Directory                       directory(uint64_t id);
        
        /*! \brief Gets directory by path
        
            This returns the directory associated by the given path.
            
            \param[in] p    The path of the directory

            \return Directory if valid, zero if not.
        */
        Directory                       directory(const std::filesystem::path& p);
        
            //std::vector<Document>      documents(Directory);   // TODO

        /*! \brief Erases the directory
        
            This removes the given directory from the database, used rarely, 
            and generally by the scanner.
            
            \parma[in] d    Directory to exist
        */
        void                            erase(Directory d);

        /*! \brief Verifies the given directory exists in the cache database
        
            \param[in] d    Directory to check
            \return TRUE if the directory is in the cache database.
        */
        bool                            exists(Directory d);

        /*! \brief Verifies the given directory's ID exists in the cache database
        
            \param[in] id   Directory ID to check
            \return TRUE if the directory is in the cache database.
        */
        bool                            exists_directory(uint64_t id);

        /*! \brief Folder for given directory
        
            This returns the folder associated with the specified directory.
            
            \param[in] d    Directory to check
            \return Folder if found, empty folder otherwise
        */
        Folder                          folder(Directory d);
        
        std::vector<Folder>             folder_path(Directory);

        /*! \brief TRUE if the directory is marked as hidden
        */
        bool                            hidden(Directory);
        
        /*! \brief One stop information
        
            This reduces the sql query count to one for the commonly needed information.
        */
        Directory::Info                 info(Directory);

        /*! \brief Key for the directory
        
            This returns the key (path relative to root) for the given directory.
        */
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
