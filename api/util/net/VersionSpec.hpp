#pragma once

#include <util/type/Version.hpp>
#include <string_view>

namespace log4cpp { class CategoryStream; }

namespace yq {

    struct VersionSpec {
        std::string_view    protocol;
        uint16_t            major = 0;
        uint16_t            minor = 0;
        
        operator Version() const { return Version { major, minor, 0, 0 }; }
        bool    operator==(const VersionSpec&) const = default;
    };
    
    inline consteval VersionSpec    http09() { return { "HTTP", 0, 9 }; }
    inline consteval VersionSpec    http10() { return { "HTTP", 1, 0 }; }
    inline consteval VersionSpec    http11() { return { "HTTP", 1, 1 }; }
    
    bool                    is_similar(const VersionSpec& a, const VersionSpec& b);
    VersionSpec             to_version_spec(const std::string_view&);
    VersionSpec             to_version_spec(const char*, size_t);
    
    class Stream;
    Stream& operator<<(Stream&, const VersionSpec&);
    log4cpp::CategoryStream& operator<<(log4cpp::CategoryStream&, const VersionSpec&);
}
