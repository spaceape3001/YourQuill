////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <util/c++/stdlibs.hpp>

    //  Linux suppourts int128
using uint128_t = unsigned __int128;
using int128_t  = __int128;

namespace log4cpp {
    class CategoryStream;
}

namespace yq {
    class ByteArray;
    class CmdArgs;
    class DelayInit;
    class Global;       //  a global property ... effectively
    
    class ArgInfo;
    class GlobalInfo;   // Meta for global
    class Meta;         // base meta 
    class MethodInfo;   // Function call
    class ObjectInfo;   // Obtrusive data type
    class PropertyInfo; // a object property
    class CompoundInfo; // CompoundInfo type (with properties & methods)   
    class TypeInfo;     // Unobtrusive data type
    
    class MetaObject;   // object with meta
    struct Parsed;
    class RefCount;
    class Stream;
    class Variant;


    /*! \brief Useful parameter for case-insensitive string keys in sets and maps
    */
    struct IgCase {
        bool    operator()(const std::string_view&, const std::string_view&) const;
    };

    /*! \brief Useful parameter for case-insensitive string keys in sets and maps
    */
    struct RevIgCase {
        bool    operator()(const std::string_view&, const std::string_view&) const;
    };
    
        //  STRUCTS
    struct Url;

        // TEMPLATE CLASS (BY CLASS)
    template <class> class EnumImpl;

        // TEMPLATE CLASSES
    template <typename> class Guarded;
    template <typename> class Ref;
    template <typename> class Touched;
    
        // TEMPLATE STRUCTS
    template <typename> struct IntRange;
    template <typename> struct Result;
    template <typename> struct Span;
    
        // COLLECTIONS
    template <typename T> class Array1;
    template <typename T> class Array2;
    template <typename T> class Deque;
    template <typename E, typename V> class EnumMap;
    template <typename K, typename V>   class Hash;
    template <typename> class List;
    template <typename K, typename V, typename C=std::less<K>> class Map;
    template <typename K, typename V, typename C=std::less<K>> class MultiMap;
    template <typename T, typename C=std::less<T>> class Set;
    template <typename T> class Stack;
    template <typename T> class Vector;
    
        // RESULTS
    
    using boolean_r         = Result<bool>;
    using double_r          = Result<double>;
    using float_r           = Result<float>;
    using int_r             = Result<int>;
    using integer_r         = Result<int>;
    using int8_r            = Result<int8_t>;
    using int16_r           = Result<int16_t>;
    using int32_r           = Result<int32_t>;
    using int64_r           = Result<int64_t>;
    using int128_r          = Result<int128_t>;
    using short_r           = Result<short>;
    using size_r            = Result<size_t>;
    using string_r          = Result<std::string>;
    using string_view_r     = Result<std::string_view>;
    //using string_map_r      = Result<StringMap>;
    //using string_set_r      = Result<StringSet>;
    using uint8_r           = Result<uint8_t>;
    using uint16_r          = Result<uint16_t>;
    using uint32_r          = Result<uint32_t>;
    using uint64_r          = Result<uint64_t>;
    using uint128_r         = Result<uint128_t>;
    using unsigned_r        = Result<unsigned int>;
    using ushort_r          = Result<unsigned short>;
    using url_r             = Result<Url>;
    using u32string_r       = Result<std::u32string>;
    using variant_r         = Result<Variant>;
    using wstring_r         = Result<std::wstring>;

        // INT RANGES
    using SSizeRange        = IntRange<ssize_t>;
    using SizeRange         = IntRange<size_t>;

        // CHARACTER SEQUQNCE
    using CharPSeq          = std::initializer_list<const char*>;

        // NOT A NUMBER
    static constexpr double   NaN     = std::numeric_limits<double>::quiet_NaN();
    static constexpr float    NaNf    = std::numeric_limits<float>::quiet_NaN();

    static constexpr double   INF     = std::numeric_limits<double>::infinity();
    static constexpr float    INFf    = std::numeric_limits<float>::infinity();

        // NO-COPY TRAIT

    class NoCopy {
        NoCopy(const NoCopy&) = delete;
        NoCopy& operator=(const NoCopy&) = delete;
    protected:
        NoCopy(){}
    };

        // NO_MOVE TRAIT

    class NoMove {
        NoMove(NoMove&&) = delete;
        NoMove& operator=(NoMove&&) = delete;
    protected:
        NoMove(){}
    };


        //  UINT 128
    struct UInt128 { 
        uint64_t low, high; 
        UInt128() : low{}, high{} {}
        UInt128(uint64_t l, uint64_t h) : low(l), high(h) {}
        operator unsigned __int128 () const { return *(const unsigned __int128*) this; }
    };


        //  Common permutations (Qt will go away...eventually)
    using StringMap             = Map<std::string,std::string,IgCase>;
    using StringMultiMap        = MultiMap<std::string,std::string,IgCase>;
    using StringSet             = Set<std::string,IgCase>;
    using StringViewSet         = Set<std::string_view,IgCase>;
    using StringPair            = std::pair<std::string,std::string>;
    using StringIntPair         = std::pair<std::string,int>;

    using string_set_t          = Set<std::string,IgCase>;
    using string_view_set_t     = Set<std::string_view,IgCase>;

    using string_view_pair_t    = std::pair<std::string_view,std::string_view>;
    using string_view_set_t     = Set<std::string_view,IgCase>;
    using string_view_vector_t  = Vector<std::string_view>;
    using path_vector_t         = Vector<std::filesystem::path>;

        // TODO move these to use std::filesystem::path
    #if defined(__APPLE__) || defined(WIN32)
        using PathSet   = StringSet;
    #elif defined(__linux__) || defined(__unix__)
        using PathSet   = Set<std::string>;
    #endif


    namespace thread {
        //! Gets the current thread's ID
        unsigned int id();
    }

    bool            is_main_thread();
    
    //!  The build directory
    const char*     build_directory();

    //!  The build's share directory
    const char*     share_directory();
}



//  To use preprocessor right, for concatenation, the double-invoke is required (nuiance)
#define YQ_CONCAT_IMPL(a, b) a##b
#define YQ_CONCAT(a, b) YQ_CONCAT_IMPL(a,b)


/*! \macro YQ_DBGREL
    \brief Simple macro for debug vs release
    \param[in]  Debug argument
    \param[in]  Release argument
*/

#ifndef NDEBUG
    #define YQ_DBGREL(a,b) a
#else
    #define YQ_DBGREL(a,b) b
#endif

