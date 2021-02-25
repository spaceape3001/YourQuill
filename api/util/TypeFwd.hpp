////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cassert>
#include <cstdint>
#include <functional>
#include <limits>
#include <memory>
#include <utility>

/*! \macro YQ_DBGREL
    \brief Simple macro for debug vs release
    \param[in]  Debug argument
    \param[in]  Release argument
*/

#ifdef _DEBUG
    #define YQ_DBGREL(a,b) a
#else
    #define YQ_DBGREL(a,b) b
#endif

struct UInt128 { 
    uint64_t low, high; 
    UInt128() : low{}, high{} {}
    UInt128(uint64_t l, uint64_t h) : low(l), high(h) {}
    operator unsigned __int128 () const { return *(const unsigned __int128*) this; }
};
using uint128_t = unsigned __int128;
using int128_t  = __int128;

class String;
class Variant;

class QByteArray;
class QDir;
class QString;
class QStringList;
class QVariant;


/*! \brief Useful parameter for case-insensitive string keys in sets and maps
*/
struct IgCase {
    bool    operator()(const String&, const String&) const;
    bool    operator()(const QString&, const QString&) const;
    bool    operator()(const QByteArray&, const QByteArray&) const;
};

/*! \brief Useful parameter for case-insensitive string keys in sets and maps
*/
struct RevIgCase {
    bool    operator()(const String&, const String&) const;
    bool    operator()(const QString&, const QString&) const;
    bool    operator()(const QByteArray&, const QByteArray&) const;
};

template <typename> class Ref;
template <typename K, typename V>   class Hash;
template <typename T> class List;
template <typename K, typename V, typename C=std::less<K>> class Map;
template <typename K, typename V, typename C=std::less<K>> class MultiMap;
template <typename T, typename C=std::less<T>> class Set;
template <typename T> class Vector;
template <typename T> class Deque;


template <class> class EnumImpl;

template <typename E, typename V> class EnumMap;

static constexpr double   NaN     = std::numeric_limits<double>::quiet_NaN();
static constexpr float    NaNf    = std::numeric_limits<float>::quiet_NaN();

template <typename T> T nan();

#define YQ_NAN( type, ... ) template <> constexpr type nan<type>() { return __VA_ARGS__; } 

YQ_NAN(float, NaNf)
YQ_NAN(double, NaN)


using StringPair            = std::pair<String,String>;
using StringMap             = Map<String,String,IgCase>;
using StringMultiMap        = MultiMap<String,String,IgCase>;
using StringSet             = Set<String,IgCase>;

using QDirVector            = Vector<QDir>;
using QStringSet            = Set<QString,IgCase>;


#if defined(__APPLE__) || defined(WIN32)
    using PathSet   = QStringSet;
#elif defined(__linux__) || defined(__unix__)
    using PathSet   = Set<QString>;
#endif

#define CONCAT(a, b) a##b

/*! \brief General dual-return of value/success 
*/
template <typename T>
struct Result {
    T       value;
    bool    good;
    
    constexpr Result() : value{}, good{} {}
    constexpr Result(const T& v) : value(v), good(true) {}
    constexpr Result(T&&v) : value(std::move(v)), good(true) {}
    constexpr Result(bool f, const T& v) : value(v), good(f) {}
    constexpr Result(bool f, T&&v) : value(std::move(v)), good(f) {}
    
    operator const T& () const { return value; }
    
    template <typename U>
    Result<U>       cast_to() const
    {
        return Result<U>(good, (U) value);
    }
    
    constexpr operator std::pair<T,bool>() const { return std::pair<T,bool>(value, good); }
};

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
using string_r          = Result<String>;
using string_map_r      = Result<StringMap>;
using string_set_r      = Result<StringSet>;
using uint8_r           = Result<uint8_t>;
using uint16_r          = Result<uint16_t>;
using uint32_r          = Result<uint32_t>;
using uint64_r          = Result<uint64_t>;
using uint128_r         = Result<uint128_t>;
using unsigned_r        = Result<unsigned int>;
using ushort_r          = Result<unsigned short>;
using qbytearray_r      = Result<QByteArray>;
using qstring_r         = Result<QString>;

unsigned int thread_id();
