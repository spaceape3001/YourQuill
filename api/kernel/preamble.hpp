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
#include <math/preamble.hpp>

namespace yq {
    // OUTSIDE DECLARATIONS
    class TypeInfo;
    class ByteArray;

    // THIS LIBRARY'S DECLARATIONS
    struct Category;
    struct Directory;
    struct Document;
    struct Folder;
    struct Fragment;
    struct Graph;
    struct Image;
    struct KVTree;
    struct KeyValue;
    struct Leaf;
    struct Root;
    struct Tag;

    template <typename> class IDLock;
    template <typename F, typename T> struct Connection;
    
    /*! \brief Association Graph Web Network
    
        This is the graph attribute database that we're shoe-horning into the cache database.  
        It's meant to abstract out bits of information as "atoms" that have properties.  The "Class" and "Field" 
        help control this data flow.
    */
    namespace agw {
    
        using atom_id   = uint64_t;
        using class_id  = uint64_t;
        using field_id  = uint64_t;
    
        struct Atom;
        struct AtomSpec;
        struct Attribute;
        struct Class;
        struct Field;
        struct Property;
        struct Value;
        
        using AtomSet           = std::set<Atom>;
        using AtomVector        = std::vector<Atom>;
        
        using AttributeVector   = std::vector<Attribute>;
        
        using ClassCountMap     = std::map<Class,HCountU64>;
        using ClassCxn          = Connection<Class,Class>;
        using ClassPair         = std::pair<Class,Class>;
        using ClassSet          = std::set<Class>;
        using ClassU64Map       = std::map<Class,uint64_t>;
        using ClassU64Pair      = std::pair<Class,uint64_t>;
        using ClassVector       = std::vector<Class>;
        
        using FieldSet          = std::set<Field>;
        using FieldVector       = std::vector<Field>;
        
        using PropertySet       = std::set<Property>;
        using PropertyVector    = std::vector<Property>;
        
        using ValueSet          = std::set<Value>;
        using ValueVector       = std::vector<Value>;
    }
    
    namespace cdb {
        struct NKI;
        class SQ;
    }

    namespace wksp {
    }

}

