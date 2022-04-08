////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <stdint.h>
#include <compare>
#include <utility>
#include <variant>
#include <yq/preamble.hpp>



namespace yq {

    struct Atom;
    struct Attribute;
    struct Class;
    struct Directory;
    struct Document;
    struct Field;
    struct Folder;
    struct Fragment;
    struct Graph;
    struct Image;
    struct Leaf;
    struct Tag;

    struct Root;

    using atom_set_t     = Set<Atom>;
    using class_set_t   = Set<Class>;
    using field_set_t   = Set<Field>;
    using tag_set_t     = Set<Tag>;

    using atom_vec_t    = std::vector<Atom>;
    using class_vec_t   = std::vector<Class>;
    using doc_vec_t     = std::vector<Document>;
    using field_vec_t   = std::vector<Field>;
    using tag_vec_t     = std::vector<Tag>;

    namespace cdb {

        enum {
        
            // Update flags
            
            //! Consider this an "update" so reset the fragment to "updated" 
            //! \note Only the scanner should be using this, may cause issues for otherrs to set this
            IsUpdate                = 0x1,
            //! Ignore attributes
            IgnoreAttributes        = 0x2,
            //! Ignore context
            IgnoreContext           = 0x4,
            
            Override                = 0x8,
            
            AllowEmpty              = 0x10
            
                
            ////  Search Flags

            ////! Include hidden things
            //Hidden                  = 0x01,

            ////! Best sort available (equiv to Sorted::YES)
            //BestSort                = 0x02
            
        };


        struct NKI;

        using DirOrFrag     = std::variant<bool,Directory,Fragment>;
        using DocOrAttr     = std::variant<bool,Document,Attribute>;
        using DocOrFold     = std::variant<bool,Document,Folder>;
        using DirOrFragStr  = std::pair<DirOrFrag,std::string>;
        using DocOrFoldStr  = std::pair<DocOrFold,std::string>;

        using DirString     = std::pair<Directory,std::string>;
        using DocString     = std::pair<Document,std::string>;
        using FoldString    = std::pair<Folder,std::string>;
        using FragString    = std::pair<Fragment,std::string>;

        using ClassPair     = std::pair<Class,Class>;
    }

}

