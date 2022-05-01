////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

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
            HIDDEN                  = 1 << 0,

            //! Best sort available (equiv to Sorted::YES)
            BEST_SORT               = 1 << 1,
            
            // Update flags
            
            //! Consider this an "update" so reset the fragment to "updated" 
            //! \note Only the scanner should be using this, may cause issues for otherrs to set this
            IS_UPDATE               = 1 << 2,
            //! Ignore attributes
            IGNORE_ATTRIBUTES       = 1 << 3,
            //! Ignore context
            IGNORE_CONTEXT          = 1 << 4,
            
            OVERRIDE                = 1 << 5,
            
            ALLOW_EMPTY             = 1 << 6,
            
            //! Skip biograph (in users/groups)
            SKIP_BIO                = 1 << 7,
            
            DONT_LOCK               = 1 << 8,

            U_ICON                  = 1 << 9,
            
            // DO INFO
            U_INFO                  = 1 << 10,
            
            U_TAGS                  = 1 << 11,
            
            U_LEAF                  = 1 << 12
            
        };
        
        using cdb_options_t         = unsigned int;
    
        /*! Base key of the argument
        
            Determines base key of the given argument, sandwiched 
            between the final '/' and the first '.' after that.
        */
        std::string_view                base_key(std::string_view);
        
        std::string                     make_filename(std::string_view k, const char* ext);
        
        template <typename C>
        std::set<uint64_t>              ids_for(const std::set<C>& cs)
        {
            std::set<uint64_t> ret;
            for(auto& c : cs)
                ret.insert(c.id);
            return ret;
        }

        template <typename C>
        std::vector<uint64_t>           ids_for(const std::vector<C>& cs)
        {
            std::vector<uint64_t> ret;
            for(auto& c : cs)
                ret.insert(c.id);
            return ret;
        }
    }
}
