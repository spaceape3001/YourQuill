////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <compare>
#include <mithril/id/Id.hpp>

namespace yq::mithril {
    struct Graphviz {
        static constexpr const IdTypeId  ID      = 16;
        static constexpr const IdTypes    PARENTS = {};

        uint64_t  id = 0ULL;
        constexpr auto    operator<=>(const Graphviz& rhs) const noexcept = default;
        constexpr operator uint64_t () const noexcept { return id; }
    };
}
YQ_TYPE_DECLARE(yq::mithril::Graphviz)
