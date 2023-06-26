////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/id/Id.hpp>

namespace yq::mithril {
    /*! \brief Document in the cache

        This represents a document in the cache, which is case insensitive and
        fused across all matching fragments in the workspace
    */
    struct Document {
        static constexpr const IdTypeId  ID      = 9;
        static constexpr const IdTypes    PARENTS = {};
        struct Info;
        uint64_t  id = 0ULL;
        constexpr auto    operator<=>(const Document&rhs) const noexcept = default;
        constexpr operator uint64_t () const noexcept { return id; }
    };
}

YQ_TYPE_DECLARE(yq::mithril::Document)
