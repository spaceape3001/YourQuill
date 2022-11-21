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
#include <basic/meta/InfoBinder.hpp>
#include <basic/Enum.hpp>
#include <math/preamble.hpp>

namespace yq {
    // OUTSIDE DECLARATIONS
    class TypeInfo;
    class ByteArray;

    // THIS LIBRARY'S DECLARATIONS
    struct Graph;
    struct Image;
    struct KVTree;
    struct KeyValue;
    struct Leaf;

    template <typename> class IDLock;
    template <typename F, typename T> struct Connection;
    

    using atom_id   = uint64_t;
    using class_id  = uint64_t;
    using field_id  = uint64_t;

    //! Generic options bitmask
    using cdb_options_t         = uint64_t;

    struct Atom;
    struct AtomSpec;
    struct Attribute;
    struct Category;
    struct Class;
    struct Directory;
    struct Document;
    struct Edge;
    struct Field;
    struct Folder;
    struct Fragment;
    struct Property;
    struct Root;
    //struct RootData;            // (might be going this way....)
    struct Tag;
    struct Value;
    
    using AtomSet               = std::set<Atom>;
    using AtomVector            = std::vector<Atom>;
    
    using AttributeVector       = std::vector<Attribute>;
    
    using CategoryVector        = std::vector<Category>;
    
    using ClassCountMap         = std::map<Class,HCountU64>;
    using ClassCxn              = Connection<Class,Class>;
    using ClassPair             = std::pair<Class,Class>;
    using ClassSet              = std::set<Class>;
    using ClassU64Map           = std::map<Class,uint64_t>;
    using ClassU64Pair          = std::pair<Class,uint64_t>;
    using ClassU64PairVector    = std::vector<ClassU64Pair>;
    using ClassVector           = std::vector<Class>;
    
    using DirectoryVector       = std::vector<Directory>;
    using DirString             = std::pair<Directory,std::string>;
    using DirOrFrag             = std::variant<bool,Directory,Fragment>;
    using DirOrFragment         = std::variant<bool,Directory,Fragment>;
    using DirOrFragStr          = std::pair<DirOrFrag,std::string>;

    using DocumentVector        = std::vector<Document>;
    using DocOrFold             = std::variant<bool,Document,Folder>;
    using DocOrFolder           = std::variant<bool,Document,Folder>;
    using DocOrFoldStr          = std::pair<DocOrFold,std::string>;
    using DocStr                = std::pair<Document,std::string>;
    using DocString             = std::pair<Document,std::string>;

    using FieldSet              = std::set<Field>;
    using FieldVector           = std::vector<Field>;
    
    using FolderStr             = std::pair<Folder,std::string>;
    using FolderVector          = std::vector<Folder>;
    using FragmentStr           = std::pair<Fragment,std::string>;
    using FragmentVector        = std::vector<Fragment>;

    using PropertySet           = std::set<Property>;
    using PropertyVector        = std::vector<Property>;
    
    using TagSet                = std::set<Tag>;
    using TagVector             = std::vector<Tag>;

    using ValueSet              = std::set<Value>;
    using ValueVector           = std::vector<Value>;
    
    struct NKI;
    namespace cdb {
        class SQ;
    }

    /*! \brief Sorted for passing into functions

        Different sort of enum, intended for function calls.
    */
    struct Sorted {
        enum Value : uint8_t { NO = 0, YES };
        Value   value;
        constexpr Sorted() : value(NO) {}
        constexpr Sorted(Value v) : value(v) {}
        operator bool() const { return value != NO; }
    };


}

