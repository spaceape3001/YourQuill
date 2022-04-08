////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <compare>
#include <cstdint>

namespace yq {
    /*! Tag in the cache database
    */
    struct Tag {
        static constexpr const char*    szExtension = "tag";
        struct Info;
        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Tag&rhs) const = default;
        constexpr operator bool() const { return id != 0ULL; }
    };
}
