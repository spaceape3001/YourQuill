////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <compare>

namespace yq {

    /*! \brief Document in the cache

        This represents a document in the cache, which is case insensitive and
        fused across all matching fragments in the workspace
    */
    struct Document {
        struct Info;
        uint64_t  id = 0ULL;
        constexpr auto    operator<=>(const Document&rhs) const = default;
        constexpr operator bool() const { return id != 0ULL; }
    };
}
