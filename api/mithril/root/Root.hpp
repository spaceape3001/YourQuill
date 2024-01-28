////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/id/Id.hpp>

namespace yq::mithril {
    /*! Tag in the cache database
    */
    struct Root {
        static constexpr const IdTypeId ID      = 23;
        static constexpr const IdTypes  PARENTS = {};

        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Root&rhs) const noexcept = default;
        constexpr operator uint64_t() const noexcept { return id; }
    };
    
}

YQ_TYPE_DECLARE(yq::mithril::Root)
