////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <mithril/preamble.hpp>

namespace yq::mithril {
    /*! \brief Place in the cache database */
    struct Place {
        uint64_t            id = 0ULL;
        using Lock = IDLock<Place>;
        constexpr auto    operator<=>(const Place&rhs) const = default; 
        constexpr operator bool() const { return id; }
    };
}

YQ_TYPE_DECLARE(yq::mithril::Place)
