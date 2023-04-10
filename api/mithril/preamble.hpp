////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <compare>
#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>
#include <meta/InfoBinder.hpp>
#include <math/preamble.hpp>
#include <mithril/keywords.hpp>

namespace yq {
    class ByteArray;
    class Stream;

    namespace mithril {
        template <typename> class IDLock;

        struct Atom;
        struct Attribute;
        struct Category;
        struct Character;
        struct Class;
        struct Directory;
        struct Document;
        struct Edge;
        struct Entity;
        struct Event;
        struct Field;
        struct Folder;
        struct Fragment;
        struct Graph;
        struct Image;
        struct Leaf;
        struct Place;
        struct Property;
        struct Root;
        struct Tag;
        struct User;

        template <typename F, typename T> struct Connection;

        struct NKI;

        using ClassPair     = std::pair<Class,Class>;
        using DirOrFrag     = std::variant<bool,Directory,Fragment>;
        using DocOrFold     = std::variant<bool,Document,Folder>;
        using DocOrFolder   = std::variant<bool,Document,Folder>;
        using DirOrFragStr  = std::pair<DirOrFrag,std::string>;
        using DocOrFoldStr  = std::pair<DocOrFold,std::string>;
        using DirString     = std::pair<Directory,std::string>;
        using DocString     = std::pair<Document,std::string>;
        using DocStr        = std::pair<Document,std::string>;
        using FolderStr     = std::pair<Folder,std::string>;
        using FragmentStr   = std::pair<Fragment,std::string>;
        using ClassSet      = std::set<Class>;
        using ClassU64Pair  = std::pair<Class,uint64_t>;
        using TagSet        = std::set<Tag>;
        
        using ClassCxn      = Connection<Class,Class>;
        using ClassU64Map   = std::map<Class,uint64_t>;
        using ClassCountMap = std::map<Class,HCountU64>;

        //! Generic options bitmask
        using cdb_options_t         = uint64_t;
        
        class CacheStatement;
        class CacheQuery;
        
        template <typename C, typename ... Params> 
        class SqlQueryMap;

        template <typename ... Params>
        using CacheQueryMap   = SqlQueryMap<CacheStatement, Params...>;

        /*! \brief Sorted for passing into functions

            Different sort of enum, intended for function calls.
        */
        struct Sorted {
            //  Note, this is done as a structure so we can do the operator bool() on it
            enum Value : uint8_t { NO = 0, YES };
            Value   value;
            constexpr Sorted() : value(NO) {}
            constexpr Sorted(Value v) : value(v) {}
            operator bool() const { return value != NO; }
        };
        
        template <typename> struct is_id_object : public std::false_type {};
        template <> struct is_id_object<Atom> : public std::true_type {};
        template <> struct is_id_object<Attribute> : public std::true_type {};
        template <> struct is_id_object<Category> : public std::true_type {};
        template <> struct is_id_object<Character> : public std::true_type {};
        template <> struct is_id_object<Class> : public std::true_type {};
        template <> struct is_id_object<Directory> : public std::true_type {};
        template <> struct is_id_object<Document> : public std::true_type {};
        template <> struct is_id_object<Edge> : public std::true_type {};
        template <> struct is_id_object<Entity> : public std::true_type {};
        template <> struct is_id_object<Event> : public std::true_type {};
        template <> struct is_id_object<Field> : public std::true_type {};
        template <> struct is_id_object<Folder> : public std::true_type {};
        template <> struct is_id_object<Fragment> : public std::true_type {};
        template <> struct is_id_object<Graph> : public std::true_type {};
        template <> struct is_id_object<Image> : public std::true_type {};
        template <> struct is_id_object<Leaf> : public std::true_type {};
        template <> struct is_id_object<Place> : public std::true_type {};
        template <> struct is_id_object<Property> : public std::true_type {};
            // Note, ROOT is not one
        template <> struct is_id_object<Tag> : public std::true_type {};
        template <> struct is_id_object<User> : public std::true_type {};
        
        template <typename T> static constexpr bool is_id_object_v = is_id_object<T>::value;


        namespace cdb {
        
            using SQ    = CacheQuery;
        
            enum {
                //! Include hidden things
                HIDDEN                  = 1 << 0,

                //! Best sort available (equiv to Sorted::YES)
                BEST_SORT               = 1 << 1,
                
                //! Startup phase (might be important)
                STARTUP                 = 1 << 2,
                
                REMOVED                 = 1 << 3,
                
                CREATED                 = 1 << 4,
                
                // Update flags
                
                //! Consider this an "update" so reset the fragment to "updated" 
                //! \note Only the scanner should be using this, may cause issues for otherrs to set this
                IS_UPDATE               = 1 << 5,
                
                //! Ignore attributes
                IGNORE_ATTRIBUTES       = 1 << 6,
                
                //! Ignore context
                IGNORE_CONTEXT          = 1 << 7,
                
                OVERRIDE                = 1 << 8,
                
                ALLOW_EMPTY             = 1 << 9,
                
                //! Skip biograph (in users/groups)
                SKIP_BIO                = 1 << 10,
                
                DONT_LOCK               = 1 << 11,

                U_ICON                  = 1 << 12,
                
                // DO INFO
                U_INFO                  = 1 << 13,
                
                U_TAGS                  = 1 << 14,
                
                U_LEAF                  = 1 << 15,
                
                U_ATTRIBUTES            = 1 << 16,
                U_EDGES                 = 1 << 17,
                
                
                ROOTS                   = 1 << 20
                
                //  Bits 24-47 are reserved for library-scoped uses
                //  Bits 48-63 are reserved for local uses
                
            };
            
        
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
            
            std::set<uint64_t>              ids_for(const std::vector<const TypeInfo*>&);

           // used for extension specifications
            struct ExtensionView {
                std::string_view        ext;
            };

            struct Extension {
                std::string             ext;
                operator ExtensionView() const noexcept { return { ext }; }
            };

            
            inline ExtensionView extension(std::string_view x) { return { x }; }
            
            /*! \brief Blindly remaps a vector of id-objects
            */
            template <typename A, typename B>
            std::vector<B>  blind_remap(const std::vector<A>& old, B)
            {
                static_assert(is_id_object_v<A>);
                static_assert(is_id_object_v<B>);
                std::vector<B>  ret;
                ret.reserve(old.size());
                for(const A& a : old)
                    ret.push_back(B{a.id});
                return ret;
            }
        }

        struct WebContext;
        struct Root;
        
        class HtmlTable;
        class WebAutoClose;
        class WebHtml;

        namespace html {
            template <typename T> struct Config;
            template <typename T> struct Dev;
            template <typename T> struct DevID;
            template <typename T> struct Edit;
            template <typename T> struct Full;
            template <typename T> struct Icon;
            template <typename T> struct LinkKey;
            template <typename T> struct LinkLabel;
            template <typename T> struct LinkThumb;
            template <typename T> struct Plural;
            template <typename T> struct Thumb;

            struct Escape;
            struct ICheck;
            struct IEditReq;
            struct IKey;
            struct IHidden;
            struct ILabel;
            struct ILine;
            struct ISubmit;
            struct IText;
            struct MarkdownText;
        }
    }
}
