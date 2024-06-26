////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>
#include <mithril/folder/Folder.hpp>
#include <vector>

#define TBL_FOLDERS "Folders"

namespace yq::mithril::cdb {

    /*! \brief ALL the folders in the cache
    
        \param[in] sorted   Flag to sort the results by key
        \return std::vector of all folders (warning, can be huge)
    */
    FolderVector             all_folders(Sorted sorted=Sorted());
    
    /*! \brief Count of folders in the cache
    */
    size_t                          all_folders_count();

    //! Brief description for folder
    std::string                     brief(Folder);
    
    //! Folder for category definition files
    consteval Folder                categories_folder() { return Folder{Folder::CATEGORIES}; }

    //! Gets named child of folder
    DocOrFold                       child(Folder, std::string_view );

    //! Number of things in folder
    size_t                          child_count(Folder);

    //! Gets named child document
    Document                        child_document(Folder, std::string_view );

    //! Gets the specified nth child of folder
    Document                        child_document(Folder, uint64_t, Sorted sorted);

    //! Gets the specified nth child of folder
    Document                        child_document(Folder, uint64_t, unsigned opts=0);

    //! Gets all documents of folder
    DocumentVector           child_documents(Folder, unsigned opts=0);

    //! Gets all documents of folder
    DocumentVector           child_documents(Folder, Sorted sorted);

    //! Gets count of child documents for folder
    size_t                          child_documents_count(Folder, unsigned opts=0);
    
    //! Gets child documents with given suffix
    DocumentVector           child_documents_by_suffix(Folder, std::string_view , Sorted sorted=Sorted());
    
    //! Gets child documens excluding given suffix
    DocumentVector           child_documents_by_suffix_excluding(Folder, std::string_view , Sorted sorted=Sorted());

    //! Gets all documents & keys of folder
    std::vector<DocString>          child_documents_with_skey(Folder, unsigned opts=0);

    //! Gets all documents & sub-keys of folder (sorted by key)
    std::vector<DocString>          child_documents_with_skey(Folder, Sorted sorted);

    //! Gets named child folder
    Folder                          child_folder(Folder, std::string_view );

    //! Gets the specified nth child of folder
    Folder                          child_folder(Folder, uint64_t, Sorted sorted);

    Folder                          child_folder(Folder, uint64_t, unsigned int opts=0);
    
    //! Gets all child folders
    FolderVector             child_folders(Folder, unsigned int opts=0);

    //! Gets all child folders
    FolderVector             child_folders(Folder, Sorted sorted);
    
    //! Counts numbers of child folders
    size_t                          child_folders_count(Folder, unsigned int opts=0);

    //! Gets all child folders
    std::vector<FolderStr>          child_folders_with_skey(Folder, unsigned int opts=0);

    //! Gets all child folders
    std::vector<FolderStr>          child_folders_with_skey(Folder, Sorted sorted);

    //! Gets child fragments
    FragmentVector           child_fragments(Folder, Sorted sorted=Sorted());
    
    
    //! Gets number of child fragments
    //! \note number of fragments may be different than documents (by design)
    size_t                          child_fragments_count(Folder);

#if 0
    std::string                     child_key(Folder);
#endif

    //! Folder for atom class definition files
    consteval Folder                classes_folder() { return Folder{Folder::CLASSES}; }
    
    //! Generic config folder used for things w/o a dedicated folder
    consteval Folder                config_folder() { return Folder{Folder::CONFIG}; }

    //std::vector<uint8_t>         data(Fragment);   // TODO

    //! Creates a folder in the cache database
    Folder                          db_folder(Folder, std::string_view , bool *wasCreated=nullptr);
    
    //! All directories that make this folder (not children)
    DirectoryVector          directories(Folder, Sorted sorted=Sorted());

    //! Directories that make this folder under specific root_dir (not children)
    DirectoryVector          directories(Folder, const RootDir*, Sorted sorted=Sorted());
    
    //! Number of directories making this folder
    size_t                          directories_count(Folder);

    //! Number of directories making this folder under specific root_dir
    //! \note On Windows, this will be no more than one, however, this can be many on any case-sensitive file system
    DirectoryVector          directories_count(Folder, const RootDir*);
    

    //! Checks for folder existence
    bool                            exists(Folder);

    //! Checks for folder existence in root_dir
    bool                            exists(Folder, const RootDir*);

    //! Checks for folder existence
    bool                            exists_folder(uint64_t);
    
    //! Folder for field definitions
    consteval Folder                fields_folder() { return Folder{Folder::FIELDS}; }

    //! First directory used to define this folder
    Directory                       first_directory(Folder);

    //! First directory used to define this folder in root_dir
    Directory                       first_directory(Folder, const RootDir*);


    /*! \brief Returns the FIRST document encountered, based on sub-key
    */
    Document                        first_document(Folder, std::initializer_list<std::string_view>);

    /*! \brief Returns the FIRST document encountered, based on sub-key
    */
    Document                        first_document(Folder, const std::vector<std::string>&);

    //! Folder that contains said fragment
    Folder                          folder(Fragment);

    //! Folder by ID
    Folder                          folder(uint64_t);
    
    //! Folder by key
    Folder                          folder(std::string_view );
    
    //! List of folders reaching specified folder
    FolderVector             folder_path(Folder);
    

    //! TRUE if folder is hidden
    bool                            hidden(Folder);
    
    //! Icon for folder
    Image                           icon(Folder);

    //! Information for folder
    Folder::Info                    info(Folder);

    //! Key for folder
    std::string                     key(Folder);

    //! Label for folder
    std::string                     label(Folder);
    
    //! Make folder's directory under root_dir (on drive)
    void                            make_directory(Folder, const RootDir*);

    //! Name for folder
    std::string                     name(Folder);

    //! Name/Key/Icon for folder
    NKI                             nki(Folder, bool autoKeyToName=false);

    //! Parent folder for folder
    Folder                          parent(Folder);

    //! TRUE if the folder's been removed from drive
    bool                            removed(Folder);
    
    //! List of root_dirs that have a directory for folder
    std::vector<const RootDir*>        root_dirs(Folder);

    //! Count of root_dirs where this folder may be found
    size_t                          roots_count(Folder);


    std::string                     skey(Folder);     //!< Key inside the directory

    std::string                     skeyb(Folder);    //!< Key inside the directory (w/o extensions)

    //! Folder for style definitions
    consteval Folder                styles_folder() { return Folder{Folder::STYLES}; }

    //! Folder for tag definitions
    consteval Folder                tags_folder() { return Folder{Folder::TAGS}; }
    
    //! Top folder (only one)
    consteval Folder                top_folder() { return Folder{Folder::TOP}; }
    
    //! Folder for user definitions
    consteval Folder                users_folder() { return Folder{Folder::USERS}; }


    /*! \brief Tests to see if directory is within the specified folder
    
        This tests to see if the spacified directory is within the folder.
        
        \param[in] parentFolder     Folder that we're checking
        \param[in] theDirectory     Directory under test
        \param[in] recursive        If TRUE, routine marches the path upward until top or parent is encountered.
        \return TRUE if the directory is within the specified folder
    */
    bool                            within(Folder parentFolder, Directory theDirectory, bool recursive=true);


    /*! \brief Tests to see if directory is within the specified folder
    
        This tests to see if the spacified directory is within the folder.
        
        \param[in] parentFolder     Folder that we're checking
        \param[in] theDocument      Document under test
        \param[in] recursive        If TRUE, routine marches the path upward until top or parent is encountered.
        \return TRUE if the directory is within the specified folder
    */
    bool                            within(Folder parentFolder, Document theDocument, bool recursive=true);

    /*! \brief Tests to see if directory is within the specified folder
    
        This tests to see if the spacified directory is within the folder.
        
        \param[in] parentFolder     Folder that we're checking
        \param[in] theFolder        Folder under test
        \param[in] recursive        If TRUE, routine marches the path upward until top or parent is encountered.
        \return TRUE if the directory is within the specified folder
    */
    bool                            within(Folder parentFolder, Folder theFolder, bool recursive=true);

    /*! \brief Tests to see if directory is within the specified folder
    
        This tests to see if the spacified directory is within the folder.
        
        \param[in] parentFolder     Folder that we're checking
        \param[in] theFragment      Fragment under test
        \param[in] recursive        If TRUE, routine marches the path upward until top or parent is encountered.
        \return TRUE if the directory is within the specified folder
    */
    bool                            within(Folder parentFolder, Fragment theFragment, bool recursive=true);
}
