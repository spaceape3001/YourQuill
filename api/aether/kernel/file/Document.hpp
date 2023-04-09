////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <aether/kernel/preamble.hpp>

namespace yq {
    /*! \brief Document in the cache

        This represents a document in the cache, which is case insensitive and
        fused across all matching fragments in the workspace
    */
    struct Document {
        struct Info;
        uint64_t  id = 0ULL;
        constexpr auto    operator<=>(const Document&rhs) const noexcept = default;
        constexpr operator uint64_t () const noexcept { return id; }
    };
}

YQ_TYPE_DECLARE(yq::Document)
