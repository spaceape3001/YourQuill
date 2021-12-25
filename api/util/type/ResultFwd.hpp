////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <stdint.h>
#include <sys/types.h>

class QByteArray;
class QString;

namespace yq {
    template <typename> struct Result;

    class String;
    class StringView;
    struct Url;

    using boolean_r         = Result<bool>;
    using double_r          = Result<double>;
    using float_r           = Result<float>;
    using int_r             = Result<int>;
    using integer_r         = Result<int>;
    using int8_r            = Result<int8_t>;
    using int16_r           = Result<int16_t>;
    using int32_r           = Result<int32_t>;
    using int64_r           = Result<int64_t>;
    //using int128_r          = Result<int128_t>;
    using short_r           = Result<short>;
    using size_r            = Result<size_t>;
    using string_r          = Result<String>;
    using string_view_r     = Result<StringView>;
    //using string_map_r      = Result<StringMap>;
    //using string_set_r      = Result<StringSet>;
    using uint8_r           = Result<uint8_t>;
    using uint16_r          = Result<uint16_t>;
    using uint32_r          = Result<uint32_t>;
    using uint64_r          = Result<uint64_t>;
    //using uint128_r         = Result<uint128_t>;
    using unsigned_r        = Result<unsigned int>;
    using ushort_r          = Result<unsigned short>;
    using qbytearray_r      = Result<QByteArray>;
    using qstring_r         = Result<QString>;
    using url_r             = Result<Url>;
}
