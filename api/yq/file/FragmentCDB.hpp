////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/db/CacheFwd.hpp>
#include <yq/file/Directory.hpp>
#include <yq/file/Document.hpp>
#include <yq/file/Folder.hpp>
#include <yq/file/Fragment.hpp>
#include <yq/enum/DataRole.hpp>
#include <yq/enum/Sorted.hpp>
#include <filesystem>

namespace yq {
    class Root;
    class ByteArray;
    
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
        std::vector<Fragment>           all_fragments(Sorted sorted=Sorted());
        size_t                          all_fragments_count();
        std::vector<Fragment>           all_fragments_suffix(std::string_view, Sorted sorted=Sorted());

        std::string                     base_key(Fragment);

    #if 0
        std::string                     child_key(Fragment);
    #endif

        //std::vector<uint8_t>         data(Fragment);   // TODO

        std::pair<Fragment, Document>   db_fragment(Directory, std::string_view , bool *wasCreated=nullptr);
        Directory                       directory(Fragment);
        Document                        document(Fragment);
        void                            erase(Fragment);
        bool                            exists(Fragment);
        bool                            exists_fragment(uint64_t);
        Folder                          folder(Fragment);
        std::vector<Folder>             folder_path(Fragment);
        
        ByteArray                       frag_bytes(Fragment, cdb_options_t opts=0);                    // reads the specified fragment
        ByteArray                       frag_bytes(std::string_view, cdb_options_t opts=0 );              // equiv to bytes(fragment(document(k)))
        ByteArray                       frag_bytes(std::string_view , DataRole, cdb_options_t opts=0);

        std::string                     frag_string(Fragment, cdb_options_t opts=0);
        std::string                     frag_string(std::string_view, cdb_options_t opts=0 );
        std::string                     frag_string(std::string_view , DataRole, cdb_options_t opts=0);

        Fragment                        fragment(const std::filesystem::path&);
        Fragment                        fragment(std::string_view );
        Fragment                        fragment(uint64_t);
        std::vector<Fragment>           fragments(std::string_view, Sorted sorted=Sorted());
        std::vector<Fragment>           fragments(std::string_view, DataRole, Sorted sorted=Sorted());
        bool                            hidden(Fragment);
        Fragment::Info                  info(Fragment);
        std::string                     key(Fragment);
        std::string                     label(Fragment);
        uint64_t                        modified(Fragment);
        std::string                     name(Fragment);
        Directory                       parent(Fragment);
        std::filesystem::path           path(Fragment);
        bool                            removed(Fragment);
        void                            rescan(Fragment);
        bool                            rescanning(Fragment);
        const Root*                     root(Fragment);
        size_t                          size(Fragment);
        std::string                     skey(Fragment);   //!< Key inside the directory
        std::string                     skeyb(Fragment);  //!< Key inside the directory (w/o extensions)
        std::string                     suffix(Fragment);
        void                            update(Fragment);
    }
}
