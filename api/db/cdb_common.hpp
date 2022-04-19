////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <string_view>
#include <utility>
#include <variant>

namespace yq {
    struct Atom;
    struct Attribute;
    struct Class;
    struct Directory;
    struct Document;
    struct Folder;
    struct Fragment;
    struct Graph;
    struct Image;
    struct Leaf;
    struct Root;
    struct Tag;
    struct User;
    class ByteArray;

    namespace cdb {

        struct NKI;

        using ClassPair     = std::pair<Class,Class>;
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
            HIDDEN                  = 0x01,

            //! Best sort available (equiv to Sorted::YES)
            BEST_SORT                = 0x02,
            
            // Update flags
            
            //! Consider this an "update" so reset the fragment to "updated" 
            //! \note Only the scanner should be using this, may cause issues for otherrs to set this
            IS_UPDATE               = 0x04,
            //! Ignore attributes
            IGNORE_ATTRIBUTES       = 0x08,
            //! Ignore context
            IGNORE_CONTEXT          = 0x10,
            
            OVERRIDE                = 0x20,
            
            ALLOW_EMPTY             = 0x40,
            
            //! Skip biograph (in users/groups)
            SKIP_BIO                = 0x80,
            
            DONT_LOCK               = 0x100
            
        };
    
        /*! Base key of the argument
        
            Determines base key of the given argument, sandwiched 
            between the final '/' and the first '.' after that.
        */
        std::string_view                base_key(std::string_view);
    }
}
