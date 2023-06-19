////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <mithril/preamble.hpp>

namespace yq::mithril {
    /*! \brief Game in the cache database */
    struct Game {
        uint64_t            id = 0ULL;
        using Lock = IDLock<Game>;
        constexpr auto    operator<=>(const Game&rhs) const = default; 
        constexpr operator bool() const { return id; }
    };
}

YQ_TYPE_DECLARE(yq::mithril::Game)
