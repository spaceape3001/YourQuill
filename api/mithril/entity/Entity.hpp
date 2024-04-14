////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/Atom.hpp>

namespace yq::mithril {
    /*! \brief Event in the cache database */
    struct Entity {
        static constexpr const IdTypeId  ID      = 10;
        static constexpr const IdTypes    PARENTS = Atom::ID;

        uint64_t            id = 0ULL;
        constexpr auto    operator<=>(const Entity&rhs) const = default; 
        constexpr operator bool() const { return id; }
    };
}

YQ_TYPE_DECLARE(yq::mithril::Entity)
YQ_TYPE_DECLARE(yq::mithril::EntitySet)
YQ_TYPE_DECLARE(yq::mithril::EntityVector)
