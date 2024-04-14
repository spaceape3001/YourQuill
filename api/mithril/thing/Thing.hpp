////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/Atom.hpp>

namespace yq::mithril {
    /*! \brief Thing in the cache database */
    struct Thing {
        static constexpr const IdTypeId ID      = 25;
        static constexpr const IdTypes  PARENTS  = Atom::ID;

        uint64_t            id = 0ULL;
        constexpr auto    operator<=>(const Thing&rhs) const = default; 
        constexpr operator uint64_t() const { return id; }
    };
}

YQ_TYPE_DECLARE(yq::mithril::Thing)
YQ_TYPE_DECLARE(yq::mithril::ThingSet)
YQ_TYPE_DECLARE(yq::mithril::ThingVector)
