////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <mithril/preamble.hpp>

namespace yq::mithril {
    /*! \brief Thing in the cache database */
    struct Thing {
        uint64_t            id = 0ULL;
        using Lock = IDLock<Thing>;
        constexpr auto    operator<=>(const Thing&rhs) const = default; 
        constexpr operator bool() const { return id; }
    };
}

YQ_TYPE_DECLARE(yq::mithril::Thing)
