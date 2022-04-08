////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Directory.hpp"
#include "Document.hpp"
#include "Folder.hpp"
#include "Fragment.hpp"

#include <yq/enum/DataRole.hpp>
#include <yq/enum/Sorted.hpp>
#include <yq/image/Image.hpp>

#include <filesystem>
#include <memory>
#include <variant>
#include <vector>


namespace yq {
    class Root;
    class ByteArray;
    

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

    struct Document::Info {
        std::string     base;
        Folder          folder;
        bool            hidden      = false;
        std::string     key;
        std::string     name;
        bool            removed     = false;
        std::string     skey;
        std::string     suffix;
        Image           icon;
        
        bool operator==(const Info&) const = default;
    };


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

    struct Fragment::Info {
        Document                document;
        Directory               directory;
        Folder                  folder;
        bool                    hidden      = false;
        uint64_t                modified    = 0ULL;
        std::string             name;
        std::filesystem::path   path;
        bool                    removed     = false;
        bool                    rescan      = false;
        const Root*             root        = nullptr;
        size_t                  size        = 0;
        
        bool operator==(const Info&) const = default;
    };

    namespace cdb {
        struct NKI;
        using DirOrFrag     = std::variant<bool,Directory,Fragment>;
        using DocOrFold     = std::variant<bool,Document,Folder>;
        using DirOrFragStr  = std::pair<DirOrFrag,std::string>;
        using DocOrFoldStr  = std::pair<DocOrFold,std::string>;
        using DirString     = std::pair<Directory,std::string>;
        using DocString     = std::pair<Document,std::string>;
        using FoldString    = std::pair<Folder,std::string>;
        using FragString    = std::pair<Fragment,std::string>;
        
        enum {
            //! Include hidden things
            Hidden                  = 0x01,

            //! Best sort available (equiv to Sorted::YES)
            BestSort                = 0x02
        };


        /*! \brief Returns all directories in the cache
        
            \param[in]  sorted  Yes/No to sort by path (default is no)
            \return std::vector of directories
        */
        std::vector<Directory>          all_directories(unsigned opts=0);
        std::vector<Directory>          all_directories(Sorted sorted);
        std::vector<Directory>          all_directories(const Root*, Sorted sorted=Sorted());
        size_t                          all_directories_count();
        size_t                          all_directories_count(const Root*);
        
        std::vector<Document>           all_documents(Sorted sorted=Sorted());
        size_t                          all_documents_count();
        std::vector<Document>           all_documents_suffix(std::string_view , Sorted sorted=Sorted());

        std::vector<Folder>             all_folders(Sorted sorted=Sorted());
        size_t                          all_folders_count();

        std::vector<Fragment>           all_fragments(Sorted sorted=Sorted());
        std::vector<Fragment>           all_fragments(const Root*,Sorted sorted=Sorted());
        size_t                          all_fragments_count();
        size_t                          all_fragments_count(const Root*);
        std::vector<Fragment>           all_fragments_suffix(std::string_view, Sorted sorted=Sorted());

        
        std::string                     base_key(Document);         //!< key w/o final extensions
        std::string                     base_key(Fragment);
        std::string_view                base_key(std::string_view);

        std::string                     brief(Folder);

        DirOrFrag                       child(Directory, std::string_view );
        DocOrFold                       child(Folder, std::string_view );

        size_t                          child_count(Directory);
        size_t                          child_count(Folder);

    #if 0
        std::string                     child_key(Directory);       //<! Key inside the directory (with extensions)
        std::string                     child_key(Document);
        std::string                     child_key(Folder);
        std::string                     child_key(Fragment);
    #endif

        std::vector<DirOrFrag>          children(Directory,Sorted sorted=Sorted());
        std::vector<DocOrFold>          children(Folder,Sorted sorted=Sorted());
        std::vector<DirOrFrag>          children(const Root*,Sorted sorted=Sorted());

        std::vector<DirOrFragStr>       children_and_names(Directory);
        std::vector<DocOrFoldStr>       children_and_names(Folder);

        consteval Folder                classes_folder() { return Folder{Folder::CLASSES}; }
        consteval Folder                config_folder() { return Folder{Folder::CONFIG}; }

        //std::vector<uint8_t>         data(Fragment);   // TODO

        std::pair<Directory, Folder>    db_directory(Directory, std::string_view , bool *wasCreated=nullptr);
        Document                        db_document(Folder, std::string_view , bool *wasCreated=nullptr);

        Folder                          db_folder(Folder, std::string_view , bool *wasCreated=nullptr);
        std::pair<Fragment, Document>   db_fragment(Directory, std::string_view , bool *wasCreated=nullptr);
        //Fragment                db_fragment(Document, const Root*);   // TODO
        Directory                       db_root(const Root*, bool *wasCreated=nullptr);


        Directory                       directory(uint64_t);
        Directory                       directory(std::string_view );
        Directory                       directory(const std::filesystem::path&);
        Directory                       directory(Fragment);
        Directory                       directory(Directory, std::string_view );
        Directory                       directory(const Root*);
        
        std::vector<Directory>          directories(Directory, Sorted sorted=Sorted());
        std::vector<Directory>          directories(Folder, Sorted sorted=Sorted());
        std::vector<Directory>          directories(const Root*, Sorted sorted=Sorted());
        
        size_t                          directories_count(Directory);
        size_t                          directories_count(Folder);
        size_t                          directories_count(const Root*);
        
        std::set<Directory>             directories_set(Directory);

        std::vector<DirString>          directories_with_names(Directory, Sorted sorted=Sorted());

        Document                        document(Folder, std::string_view );
        Document                        document(Fragment);

        Document                        document(std::string_view );
        Document                        document(uint64_t);
        
        /*! \brief Returns the FIRST document encountered
        */
        Document                        document(std::initializer_list<std::string_view>);

            //std::vector<Document>      documents(Directory);   // TODO
        std::vector<Document>           documents(Folder, unsigned opts=0);
        std::vector<Document>           documents(Folder, Sorted sorted);
        size_t                          documents_count(Folder, unsigned opts=0);
        std::vector<Document>           documents_by_suffix(Folder, std::string_view , Sorted sorted=Sorted());
        std::vector<Document>           documents_by_suffix_excluding(Folder, std::string_view , Sorted sorted=Sorted());

        void                            erase(Directory);
        void                            erase(Fragment);

        bool                            exists(Directory);
        bool                            exists(Document);
        bool                            exists(Folder);
        bool                            exists(Folder, const Root*);
        bool                            exists(Fragment);

        bool                            exists(const Root*, std::string_view );

        bool                            exists_directory(uint64_t);
        bool                            exists_document(uint64_t);
        bool                            exists_folder(uint64_t);
        bool                            exists_fragment(uint64_t);

        Fragment                        first(Document);
        Fragment                        first(Document, const Root*);
        Fragment                        first(Document, DataRole);

        Folder                          folder(Directory);
        Folder                          folder(Document);
        Folder                          folder(Fragment);
        Folder                          folder(Folder, std::string_view );

        Folder                          folder(uint64_t);
        Folder                          folder(std::string_view );
        
        std::vector<Folder>             folder_path(Directory);
        std::vector<Folder>             folder_path(Document);
        std::vector<Folder>             folder_path(Folder);
        std::vector<Folder>             folder_path(Fragment);
        
        std::vector<Folder>             folders(Folder, unsigned int opts=0);
        std::vector<Folder>             folders(Folder, Sorted sorted);
        size_t                          folders_count(Folder, unsigned int opts=0);

        ByteArray                       frag_bytes(Fragment);                    // reads the specified fragment
        ByteArray                       frag_bytes(std::string_view );              // equiv to bytes(fragment(document(k)))
        ByteArray                       frag_bytes(std::string_view , DataRole);

        std::string                     frag_string(Fragment);
        std::string                     frag_string(std::string_view );
        std::string                     frag_string(std::string_view , DataRole);

        Fragment                        fragment(const std::filesystem::path&);
        Fragment                        fragment(std::string_view );
        Fragment                        fragment(uint64_t);
        Fragment                        fragment(const Root*, std::string_view );
        Fragment                        fragment(Directory, std::string_view );
        
        /*! \brief Returns the first fragmment found for the given document
        */
        Fragment                        fragment(Document);
        Fragment                        fragment(Document, const Root*);
        Fragment                        fragment(Document, DataRole);
        
        std::vector<Fragment>           fragments(Document, Sorted sorted=Sorted());
        std::vector<Fragment>           fragments(Document, Sorted::Value sorted);
        std::vector<Fragment>           fragments(Document, const Root*, Sorted sorted=Sorted());
        std::vector<Fragment>           fragments(Document, const Root*, Sorted::Value sorted);
        std::vector<Fragment>           fragments(Document, DataRole, Sorted sorted=Sorted());
        std::vector<Fragment>           fragments(Directory, Sorted sorted=Sorted());
        std::vector<Fragment>           fragments(Folder, Sorted sorted=Sorted());
        std::vector<Fragment>           fragments(const Root*, Sorted sorted=Sorted());
        std::vector<Fragment>           fragments(std::string_view , Sorted sorted=Sorted());
        std::vector<Fragment>           fragments(std::string_view , DataRole, Sorted sorted=Sorted());

        size_t                          fragments_count(Document);
        size_t                          fragments_count(Document, const Root*);
        size_t                          fragments_count(Directory);
        size_t                          fragments_count(Folder);

        std::set<Fragment>              fragments_set(Directory);

        bool                            hidden(Directory);
        bool                            hidden(Document);
        bool                            hidden(Folder);
        bool                            hidden(Fragment);
        
        void                            hide(Document);

        Image                           icon(Document);
        Image                           icon(Folder);

        Directory::Info                 info(Directory);
        Document::Info                  info(Document);
        Folder::Info                    info(Folder);
        Fragment::Info                  info(Fragment);

        std::string                     key(Directory);
        std::string                     key(Document);
        std::string                     key(Folder);
        std::string                     key(Fragment);
        std::string                     key(const Root*);

        //std::string                 label(Directory);
        std::string                     label(Document);
        std::string                     label(Folder);
        std::string                     label(Fragment);
        
        void                            make_directory(Folder, const Root*);

        uint64_t                        modified(Fragment);

        std::string                     name(Directory);
        std::string                     name(Document);
        std::string                     name(Folder);
        std::string                     name(Fragment);

        NKI                             nki(Document, bool autoKeyToName=false);
        NKI                             nki(Folder, bool autoKeyToName=false);


        Directory                       parent(Directory);
        Folder                          parent(Document);
        Folder                          parent(Folder);
        Directory                       parent(Fragment);

        std::filesystem::path           path(Directory);
        std::filesystem::path           path(Fragment);
        std::filesystem::path           path(const Root*, std::string_view , bool fMakePath=false);
        std::filesystem::path           path(const Root*, Document, bool fMakePath=false);

        bool                            removed(Directory);
        bool                            removed(Document);
        bool                            removed(Folder);
        bool                            removed(Fragment);
        
        void                            rescan(Fragment);
        bool                            rescanning(Fragment);

        const Root*                     root(Directory);
        const Root*                     root(Fragment);
        
        std::vector<const Root*>        roots(Document);
        std::vector<const Root*>        roots(Folder);

        size_t                          roots_count(Document);
        size_t                          roots_count(Folder);

        void                            show(Document);
        size_t                          size(Fragment);

        std::string                     skey(Directory);  //!< Key inside the directory
        std::string                     skey(Document);   //!< Key inside the directory
        std::string                     skey(Folder);     //!< Key inside the directory
        std::string                     skey(Fragment);   //!< Key inside the directory

        std::string                     skeyb(Directory); //!< Key inside the directory (w/o extensions)
        std::string                     skeyb(Document);  //!< Key inside the directory (w/o extensions)
        std::string                     skeyb(Folder);    //!< Key inside the directory (w/o extensions)
        std::string                     skeyb(Fragment);  //!< Key inside the directory (w/o extensions)

        std::string                     suffix(Document);
        std::string                     suffix(Fragment);

        consteval Folder                tags_folder() { return Folder{Folder::TAGS}; }
        consteval Folder                top_folder() { return Folder{Folder::TOP}; }

        void                            update(Fragment);

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

        Fragment                        writable(Document, DataRole);
    }
}
