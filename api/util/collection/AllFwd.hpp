////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <functional>

    //  Lingering Qt....
class QString;
//template <typename> class QList;

namespace yq {
    struct IgCase;
    class String;   // until I want to move this beast....

    template <typename T> class Array1;
    template <typename T> class Array2;
    template <typename T> class Deque;
    template <class> class EnumImpl;
    template <typename E, typename V> class EnumMap;
    template <typename K, typename V>   class Hash;
    template <typename> class List;
    template <typename K, typename V, typename C=std::less<K>> class Map;
    template <typename K, typename V, typename C=std::less<K>> class MultiMap;
    template <typename T, typename C=std::less<T>> class Set;
    template <typename T> class Stack;
    template <typename T> class Vector;

    using StringMap             = Map<String,String,IgCase>;
    using StringMultiMap        = MultiMap<String,String,IgCase>;
    using StringSet             = Set<String,IgCase>;
    using QStringSet            = Set<QString,IgCase>;

    #if defined(__APPLE__) || defined(WIN32)
        using QPathSet   = QStringSet;
    #elif defined(__linux__) || defined(__unix__)
        using QPathSet   = Set<QString>;
    #endif

}


