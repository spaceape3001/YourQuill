////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/Atom.hpp>

namespace yq::mithril {
    /*! \brief Game in the cache database */
    struct Game {
        static constexpr const IdTypeId  ID      = 14;
        static constexpr const IdTypes   PARENTS = Atom::ID;

        uint64_t            id = 0ULL;
        constexpr auto    operator<=>(const Game&rhs) const = default; 
        constexpr operator uint64_t() const { return id; }
    };
}

YQ_TYPE_DECLARE(yq::mithril::Game)
