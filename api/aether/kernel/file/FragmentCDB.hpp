////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/preamble.hpp>
#include <kernel/file/Directory.hpp>
#include <kernel/file/Document.hpp>
#include <kernel/file/Folder.hpp>
#include <kernel/file/Fragment.hpp>
#include <kernel/enum/DataRole.hpp>

#include <filesystem>

namespace yq {
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
    
        //! List of all fragments
        std::vector<Fragment>           all_fragments(Sorted sorted=Sorted());
        
        //! Count of all fragments
        size_t                          all_fragments_count();
        
        //! All fragments with given suffix
        std::vector<Fragment>           all_fragments_suffix(std::string_view, Sorted sorted=Sorted());

        //! Base key (no-suffix) of fragment
        std::string                     base_key(Fragment);

    #if 0
        std::string                     child_key(Fragment);
    #endif

        //std::vector<uint8_t>         data(Fragment);   // TODO

        //! Creates fragment/document
        std::pair<Fragment, Document>   db_fragment(Directory, std::string_view , bool *wasCreated=nullptr);
        
        //! Directory containing fragment
        Directory                       directory(Fragment);
        
        //! Associated document for fragment
        Document                        document(Fragment);
        
        //! Erase fragment from cache database
        void                            erase(Fragment);
        
        //! TRUE if the fragment exists
        bool                            exists(Fragment);

        //! TRUE if the fragment exists
        bool                            exists_fragment(uint64_t);
        
        //! Containing folder for fragment
        Folder                          folder(Fragment);
        
        //! Path in folders to fragment
        std::vector<Folder>             folder_path(Fragment);
        
        //! Gets the fragment's contents as a byte array
        ByteArray                       frag_bytes(Fragment, cdb_options_t opts=0);                    // reads the specified fragment

        //! Gets the contents of the first matching fragment as a byte array
        ByteArray                       frag_bytes(std::string_view, cdb_options_t opts=0 );              // equiv to bytes(fragment(document(k)))

        //! Gets the contents of the first matching fragment as a byte array
        ByteArray                       frag_bytes(std::string_view , DataRole, cdb_options_t opts=0);

        //! Gets the fragments contents as a std::string
        std::string                     frag_string(Fragment, cdb_options_t opts=0);
        
        //! Gets the contents of the first matching fragment as a string
        std::string                     frag_string(std::string_view, cdb_options_t opts=0 );

        //! Gets the contents of the first matching fragment as a string
        std::string                     frag_string(std::string_view , DataRole, cdb_options_t opts=0);

        //! Fragment for path
        Fragment                        fragment(const std::filesystem::path&);
        //! Fragment for path
        Fragment                        fragment(std::string_view );
        //! Fragment for ID
        Fragment                        fragment(uint64_t);
        
        //! Fragments for key
        std::vector<Fragment>           fragments(std::string_view, Sorted sorted=Sorted());
        
        //! Fragments for key
        std::vector<Fragment>           fragments(std::string_view, DataRole, Sorted sorted=Sorted());
        
        //! TRUE if the fragment is hidden
        bool                            hidden(Fragment);
        
        //! Info for fragment
        Fragment::Info                  info(Fragment);
        
        //! Key for fragment
        std::string                     key(Fragment);
        
        //! Label for fragment
        std::string                     label(Fragment);
        
        //! Last Modified time code
        uint64_t                        modified(Fragment);
        
        //! Name of this fragment
        std::string                     name(Fragment);
        
        //! Containing directory for fragment
        Directory                       parent(Fragment);
        
        //! Path for the fragment
        std::filesystem::path           path(Fragment);
        
        //! TRUE if fragment's been removed from the drive
        bool                            removed(Fragment);
        
        //! Instructs the scanner to rescan this fragment
        void                            rescan(Fragment);
        
        //! TRUE if fragment has been flagged for rescanning
        bool                            rescanning(Fragment);
        
        //! Root for this fragment
        const Root*                     root(Fragment);
        
        //! Recorded file size for fragment
        size_t                          size(Fragment);
        
        //! Portion of the key within directory
        std::string                     skey(Fragment);   //!< Key inside the directory

        //! Portion of the key within directory w/o extensions
        std::string                     skeyb(Fragment);  //!< Key inside the directory (w/o extensions)
        
        //! File extension for fragment
        Extension                       suffix(Fragment);
        
        //! Update fragment information in cache database
        void                            update(Fragment);
    }
}
