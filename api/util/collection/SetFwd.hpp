////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <functional>

template <typename T, typename C=std::less<T>> class Set;

struct IgCase;
class QString;
class String;

using StringSet             = Set<String,IgCase>;
using QStringSet            = Set<QString,IgCase>;

#if defined(__APPLE__) || defined(WIN32)
    using PathSet   = QStringSet;
#elif defined(__linux__) || defined(__unix__)
    using PathSet   = Set<QString>;
#endif

