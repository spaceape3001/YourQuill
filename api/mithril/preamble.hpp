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
#include <0/meta/InfoBinder.hpp>
#include <0/math/preamble.hpp>

namespace yq {
    class ByteArray;
    class Stream;
    class Meta;
}

namespace yq::mithril {
    using id_t      = uint64_t;
    
    using MetaBase = yq::Meta;
    
    struct Id;

    struct Atom;
    struct Attribute;
    struct Book;
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
    struct Game;
    struct Graph;       //< TBD
    struct Graphviz;    //< Prerendered graphviz graphs
    struct Group;
    struct Image;
    struct Leaf;
    struct Meta;
    struct Organization;
    struct Place;
    struct Property;
    struct Root;
    struct RootDir;
    struct Style;
    struct Tag;
    struct Thing;
    struct User;
    struct Value;

    template <typename F, typename T> struct Connection;
    template <typename...>  class Notifier;
    template <typename> struct Changed;

    struct NKI;
    using HopCount              = HCountU8;
    using hop_t                 = uint8_t;


    using AtomSet               = std::set<Atom>;
    using AtomVector            = std::vector<Atom>;
    
    using AttributeSet          = std::set<Attribute>;
    using AttributeVector       = std::vector<Attribute>;
    
    using BookSet               = std::set<Book>;
    using BookVector            = std::vector<Book>;
    
    using CategorySet           = std::set<Category>;
    using CategoryVector        = std::vector<Category>;
    
    using CharacterSet          = std::set<Character>;
    using CharacterVector       = std::vector<Character>;

    using ClassCountMap         = std::map<Class,HCountU64>;
    using ClassCxn              = Connection<Class,Class>;
    using ClassHopMap           = std::map<Class,HopCount>;
    using ClassPair             = std::pair<Class,Class>;
    using ClassPairSet          = std::set<ClassPair>;
    using ClassSet              = std::set<Class>;
    using ClassU64Pair          = std::pair<Class,uint64_t>;
    using ClassVector           = std::vector<Class>;
    using ClassU64Map           = std::map<Class,uint64_t>;
    
    using DirectorySet          = std::set<Directory>;
    using DirectoryVector       = std::vector<Directory>;
    
    using DirOrFrag             = std::variant<bool,Directory,Fragment>;
    using DirOrFragStr          = std::pair<DirOrFrag,std::string>;
    using DirString             = std::pair<Directory,std::string>;
    
    using DocumentSet           = std::set<Document>;
    using DocumentVector        = std::vector<Document>;

    using DocOrFold             = std::variant<bool,Document,Folder>;
    using DocOrFolder           = std::variant<bool,Document,Folder>;
    using DocOrFoldStr          = std::pair<DocOrFold,std::string>;
    using DocString             = std::pair<Document,std::string>;
    using DocStr                = std::pair<Document,std::string>;
    
    using EntitySet             = std::set<Entity>;
    using EntityVector          = std::vector<Entity>;
    
    using EventSet              = std::set<Event>;
    using EventVector           = std::vector<Event>;

    using FieldHopMap           = std::map<Field,HopCount>;
    using FieldSet              = std::set<Field>;
    using FieldVector           = std::vector<Field>;

    using FolderSet             = std::set<Folder>;
    using FolderStr             = std::pair<Folder,std::string>;
    using FolderVector          = std::vector<Folder>;
    
    using FragmentSet           = std::set<Fragment>;
    using FragmentStr           = std::pair<Fragment,std::string>;
    using FragmentVector        = std::vector<Fragment>;
    
    using GameSet               = std::set<Game>;
    using GameVector            = std::vector<Game>;
    
    using GraphvizSet           = std::set<Graphviz>;
    using GraphvizVector        = std::vector<Graphviz>;
    
    using GroupSet              = std::set<Group>;
    using GroupVector           = std::vector<Group>;
    
    using ImageSet              = std::set<Image>;
    using ImageVector           = std::vector<Image>;
    
    using LeafSet               = std::set<Leaf>;
    using LeafVector            = std::vector<Leaf>;
    
    using OrganizationSet       = std::set<Organization>;
    using OrganizationVector    = std::vector<Organization>;
    
    using PlaceSet              = std::set<Place>;
    using PlaceVector           = std::vector<Place>;
    
    using StyleSet              = std::set<Style>;
    using StyleVector           = std::vector<Style>;

    using TagSet                = std::set<Tag>;
    using TagVector             = std::vector<Tag>;
    
    using ThingSet              = std::set<Thing>;
    using ThingVector           = std::vector<Thing>;
    
    using UserSet               = std::set<User>;
    using UserVector            = std::vector<User>;
    
    using ValueSet              = std::set<Value>;
    using ValueVector           = std::vector<Value>;




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
        

       // used for extension specifications
        struct ExtensionView {
            std::string_view        ext;
        };

        struct Extension {
            std::string             ext;
            operator ExtensionView() const noexcept { return { ext }; }
        };

        
        inline ExtensionView extension(std::string_view x) { return { x }; }
        
    }

    struct WebContext;
    
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
