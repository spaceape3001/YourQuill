#pragma once

#include <util/Enum.hpp>

#include <filesystem>
#include <string>
#include <string_view>

namespace yq {
    class Stream;

    Stream&     operator<<(Stream&, const char*);
    Stream&     operator<<(Stream&, const std::string&);
    Stream&     operator<<(Stream&, const std::string_view&);
    Stream&     operator<<(Stream&, const QString&);
    Stream&     operator<<(Stream&, const QByteArray&);
    Stream&     operator<<(Stream&, char);

    Stream&     operator<<(Stream&, float);
    Stream&     operator<<(Stream&, double);

    Stream&     operator<<(Stream&, bool);

    Stream&     operator<<(Stream&, int8_t);
    Stream&     operator<<(Stream&, int16_t);
    Stream&     operator<<(Stream&, int32_t);
    Stream&     operator<<(Stream&, int64_t);

    Stream&     operator<<(Stream&, uint8_t);
    Stream&     operator<<(Stream&, uint16_t);
    Stream&     operator<<(Stream&, uint32_t);
    Stream&     operator<<(Stream&, uint64_t);

    Stream&     operator<<(Stream&, const std::filesystem::path&);

    template <typename E>
    Stream&     operator<<(Stream& str, EnumImpl<E> val)
    {
        return str << val.key();
    }
}
