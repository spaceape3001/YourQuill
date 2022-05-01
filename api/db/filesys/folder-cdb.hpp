////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>

namespace yq {
    class Root;
    class ByteArray;
    

    struct Folder::Info {
        std::string     brief;
        bool            hidden      = false;
        std::string     key;
        std::string     name;
        Folder          parent;
        bool            removed     = false;
        std::string     skey;
        Image           icon;

        bool operator==(const Info&) const = default;
    };

    namespace cdb {

        /*! \brief ALL the folders in the cache
        
            \param[in] sorted   Flag to sort the results by key
            \return std::vector of all folders (warning, can be huge)
        */
        std::vector<Folder>             all_folders(Sorted sorted=Sorted());
        
        /*! \brief Count of folders in the cache
        */
        size_t                          all_folders_count();


        std::string                     brief(Folder);
        
        
        consteval Folder                categories_folder() { return Folder{Folder::CATEGORIES}; }

        DocOrFold                       child(Folder, std::string_view );

        size_t                          child_count(Folder);

        Document                        child_document(Folder, std::string_view );

        std::vector<Document>           child_documents(Folder, unsigned opts=0);
        std::vector<Document>           child_documents(Folder, Sorted sorted);
        size_t                          child_documents_count(Folder, unsigned opts=0);
        std::vector<Document>           child_documents_by_suffix(Folder, std::string_view , Sorted sorted=Sorted());
        std::vector<Document>           child_documents_by_suffix_excluding(Folder, std::string_view , Sorted sorted=Sorted());

        Folder                          child_folder(Folder, std::string_view );

        std::vector<Folder>             child_folders(Folder, unsigned int opts=0);
        std::vector<Folder>             child_folders(Folder, Sorted sorted);
        size_t                          child_folders_count(Folder, unsigned int opts=0);


        std::vector<Fragment>           child_fragments(Folder, Sorted sorted=Sorted());
        size_t                          child_fragments_count(Folder);

    #if 0
        std::string                     child_key(Folder);
    #endif

        std::vector<DocOrFold>          children(Folder,Sorted sorted=Sorted());

        std::vector<DocOrFoldStr>       children_and_names(Folder);

        consteval Folder                classes_folder() { return Folder{Folder::CLASSES}; }
        consteval Folder                config_folder() { return Folder{Folder::CONFIG}; }

        //std::vector<uint8_t>         data(Fragment);   // TODO


        Folder                          db_folder(Folder, std::string_view , bool *wasCreated=nullptr);
        
        std::vector<Directory>          directories(Folder, Sorted sorted=Sorted());
        std::vector<Directory>          directories(Folder, const Root*, Sorted sorted=Sorted());
        
        size_t                          directories_count(Folder);
        std::vector<Directory>          directories_count(Folder, const Root*);
        

        Document                        document(std::string_view );
        Document                        document(uint64_t);
        


        bool                            exists(Folder);
        bool                            exists(Folder, const Root*);

        bool                            exists_folder(uint64_t);
        
        consteval Folder                fields_folder() { return Folder{Folder::FIELDS}; }

        Directory                       first_directory(Folder);
        Directory                       first_directory(Folder, const Root*);


        /*! \brief Returns the FIRST document encountered
        */
        Document                        first_document(Folder, std::initializer_list<std::string_view>);

        Folder                          folder(Fragment);

        Folder                          folder(uint64_t);
        Folder                          folder(std::string_view );
        
        std::vector<Folder>             folder_path(Folder);
        


        bool                            hidden(Folder);
        
        Image                           icon(Folder);

        Folder::Info                    info(Folder);

        std::string                     key(Folder);

        std::string                     label(Folder);
        
        void                            make_directory(Folder, const Root*);


        std::string                     name(Folder);

        NKI                             nki(Folder, bool autoKeyToName=false);

        Folder                          parent(Folder);

        bool                            removed(Folder);
        
        
        std::vector<const Root*>        roots(Folder);

        size_t                          roots_count(Folder);


        std::string                     skey(Folder);     //!< Key inside the directory

        std::string                     skeyb(Folder);    //!< Key inside the directory (w/o extensions)


        consteval Folder                tags_folder() { return Folder{Folder::TAGS}; }
        consteval Folder                top_folder() { return Folder{Folder::TOP}; }
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
}
