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
    struct KeyValue;
    template <typename> class Ref;
    
    struct atom_t {};
    struct attribute_t {};
    struct book_t {};
    struct category_t {};
    struct character_t {};
    struct document_t {};
    struct directory_t {};
    struct doodle_t {};
    struct entity_t {};
    struct event_t {};
    struct field_t {};
    struct folder_t {};
    struct fragment_t {};
    struct game_t {};
    struct graphviz_t {};
    struct group_t {};
    struct image_t {};
    struct leaf_t {};
    struct organization_t {};
    struct novel_t {};
    struct place_t {};
    struct parent_t {};
    struct root_t {};
    struct style_t {};
    struct suffix_t {};
    struct tag_t {};
    struct template_t {};
    struct thing_t {};
    struct user_t {};
    struct value_t {};
    
    static constexpr atom_t         ATOM;
    static constexpr attribute_t    ATTRIBUTE;
    static constexpr book_t         BOOK;
    static constexpr category_t     CATEGORY;
    static constexpr character_t    CHARACTER;
    static constexpr directory_t    DIRECTORY;
    static constexpr document_t     DOCUMENT;
    static constexpr doodle_t       DOODLE;
    static constexpr entity_t       ENTITY;
    static constexpr event_t        EVENT;
    static constexpr field_t        FIELD;
    static constexpr folder_t       FOLDER;
    static constexpr fragment_t     FRAGMENT;
    static constexpr game_t         GAME;
    static constexpr graphviz_t     GRAPHVIZ;
    static constexpr group_t        GROUP;
    static constexpr image_t        IMAGE;
    static constexpr leaf_t         LEAF;
    static constexpr novel_t        NOVEL;
    static constexpr organization_t ORGANIZATION;
    static constexpr parent_t       PARENT;
    static constexpr place_t        PLACE;
    static constexpr root_t         ROOT;
    static constexpr style_t        STYLE;
    static constexpr suffix_t       SUFFIX;
    static constexpr tag_t          TAG;
    static constexpr template_t     TEMPLATE;
    static constexpr thing_t        THING;
    static constexpr user_t         USER;
    static constexpr value_t        VALUE;
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
    struct Doodle;
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
    struct Novel;
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

    class D3Object;

    template <typename F, typename T> struct Connection;
    template <typename...>  class Notifier;
    template <typename> struct Changed;
    template <typename> struct WebSearcher;

    using D3CPtr                = Ref<const D3Object>;
    using D3Ptr                 = Ref<D3Object>;

    struct NKI;
    using HopCount              = HCountU8;
    using hop_t                 = uint8_t;


    using AtomCSpan             = std::span<const Atom>;
    using AtomSet               = std::set<Atom>;
    using AtomVector            = std::vector<Atom>;
    
    using AttributeCSpan        = std::span<const Attribute>;
    using AttributeSet          = std::set<Attribute>;
    using AttributeVector       = std::vector<Attribute>;
    
    using BookCSpan             = std::span<const Book>;
    using BookSet               = std::set<Book>;
    using BookVector            = std::vector<Book>;
    
    using CategoryCSpan         = std::span<const Category>;
    using CategorySet           = std::set<Category>;
    using CategoryVector        = std::vector<Category>;
    
    using CharacterCSpan        = std::span<const Character>;
    using CharacterSet          = std::set<Character>;
    using CharacterVector       = std::vector<Character>;

    using ClassCSpan            = std::span<const Class>;
    using ClassCountMap         = std::map<Class,HCountU64>;
    using ClassCxn              = Connection<Class,Class>;
    using ClassHopMap           = std::map<Class,HopCount>;
    using ClassPair             = std::pair<Class,Class>;
    using ClassPairSet          = std::set<ClassPair>;
    using ClassSet              = std::set<Class>;
    using ClassU64Pair          = std::pair<Class,uint64_t>;
    using ClassVector           = std::vector<Class>;
    using ClassU64Map           = std::map<Class,uint64_t>;
   
    using DirectoryCSpan        = std::span<const Directory>;
    using DirectorySet          = std::set<Directory>;
    using DirectoryVector       = std::vector<Directory>;
    
    using DirOrFrag             = std::variant<bool,Directory,Fragment>;
    using DirOrFragStr          = std::pair<DirOrFrag,std::string>;
    using DirString             = std::pair<Directory,std::string>;
    
    using DocumentCSpan         = std::span<const Document>;
    using DocumentSet           = std::set<Document>;
    using DocumentVector        = std::vector<Document>;

    using DocOrFold             = std::variant<bool,Document,Folder>;
    using DocOrFolder           = std::variant<bool,Document,Folder>;
    using DocOrFoldStr          = std::pair<DocOrFold,std::string>;
    using DocString             = std::pair<Document,std::string>;
    using DocStr                = std::pair<Document,std::string>;
    
    using DoodleCSpan          = std::span<const Doodle>;
    using DoodleSet            = std::set<Doodle>;
    using DoodleVector         = std::vector<Doodle>;
    
    using EntityCSpan           = std::span<const Entity>;
    using EntitySet             = std::set<Entity>;
    using EntityVector          = std::vector<Entity>;
    
    using EventCSpan            = std::span<const Event>;
    using EventSet              = std::set<Event>;
    using EventVector           = std::vector<Event>;

    using FieldCSpan            = std::span<const Field>;
    using FieldHopMap           = std::map<Field,HopCount>;
    using FieldSet              = std::set<Field>;
    using FieldVector           = std::vector<Field>;

    using FolderCSpan           = std::span<const Folder>;
    using FolderSet             = std::set<Folder>;
    using FolderStr             = std::pair<Folder,std::string>;
    using FolderVector          = std::vector<Folder>;
    
    using FragmentCSpan         = std::span<const Fragment>;
    using FragmentSet           = std::set<Fragment>;
    using FragmentStr           = std::pair<Fragment,std::string>;
    using FragmentVector        = std::vector<Fragment>;
    
    using GameCSpan             = std::span<const Game>;
    using GameSet               = std::set<Game>;
    using GameVector            = std::vector<Game>;
    
    using GraphvizCSpan         = std::span<const Graphviz>;
    using GraphvizSet           = std::set<Graphviz>;
    using GraphvizVector        = std::vector<Graphviz>;
    
    using GroupCSpan            = std::span<const Group>;
    using GroupSet              = std::set<Group>;
    using GroupVector           = std::vector<Group>;
    
    using ImageCSpan            = std::span<const Image>;
    using ImageSet              = std::set<Image>;
    using ImageVector           = std::vector<Image>;
    
    using LeafCSpan             = std::span<const Leaf>;
    using LeafSet               = std::set<Leaf>;
    using LeafVector            = std::vector<Leaf>;
    
    using NovelCSpan            = std::span<const Novel>;
    using NovelSet              = std::set<Novel>;
    using NovelVector           = std::vector<Novel>;

    using OrganizationCSpan     = std::span<const Organization>;
    using OrganizationSet       = std::set<Organization>;
    using OrganizationVector    = std::vector<Organization>;
    
    using PlaceCSpan            = std::span<const Place>;
    using PlaceSet              = std::set<Place>;
    using PlaceVector           = std::vector<Place>;

    using RootDirVector         = std::vector<const RootDir*>;
    
    using RootVector            = std::vector<Root>;
    
    using StyleCSpan            = std::span<const Style>;
    using StyleSet              = std::set<Style>;
    using StyleVector           = std::vector<Style>;

    using TagCSpan              = std::span<const Tag>;
    using TagSet                = std::set<Tag>;
    using TagVector             = std::vector<Tag>;
    
    using ThingCSpan            = std::span<const Thing>;
    using ThingSet              = std::set<Thing>;
    using ThingVector           = std::vector<Thing>;
    
    using UserCSpan             = std::span<const User>;
    using UserSet               = std::set<User>;
    using UserVector            = std::vector<User>;
    
    using ValueCSpan            = std::span<const Value>;
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
        consteval Sorted() noexcept : value(NO) {}
        constexpr Sorted(Value v) noexcept: value(v) {}
        constexpr operator bool() const noexcept { return value != NO; }
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
