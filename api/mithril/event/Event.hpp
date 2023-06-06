////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <mithril/preamble.hpp>

namespace yq::mithril {
    /*! \brief Event in the cache database */
    struct Event {
        using Lock = IDLock<Event>;
        uint64_t            id = 0ULL;
        constexpr auto    operator<=>(const Event&rhs) const = default; 
        constexpr operator bool() const { return id; }
    };
}

YQ_TYPE_DECLARE(yq::mithril::Event)
