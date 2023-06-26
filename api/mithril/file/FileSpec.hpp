////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <mithril/preamble.hpp>

namespace yq::mithril {
    struct Folder;
    struct Class;
    struct Fragment;
    struct Id;
    
    struct FileSpec {
        enum Type {
            Never    = 0,       //<! Never detect
            Always,             //<! Always detect
            ByFile,             //!< By filepath
            ByExtension,        //!< By file extension
            ByCacheDoc,         //!< Match the name (any folder)
            ByCacheExt,         //!< By file extension (any folder)
            ByFolder,           //!< All in Folder
            ByFolderExt,        //!< By folder + extension
            ByFolderDoc         //!< By folder + document name
        };
    
        std::string             str;
        uint64_t                u64     = 0;
        Type                    type    = Never;
        
        bool    match(Id) const;
        
        //bool    match(Fragment) const;
        bool    match(Folder, std::string_view) const;
        bool    match(const std::filesystem::path&) const;
    };
    
    FileSpec        by_file(const std::filesystem::path&);
    FileSpec        by_cache(std::string_view);
    FileSpec        by_cache(Folder);
    FileSpec        by_cache(Folder, std::string_view);
}
