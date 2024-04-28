////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/Atom.hpp>

namespace yq::mithril {
    /*! \brief Place in the cache database */
    struct Place {
        static constexpr const IdTypeId ID      = 23;
        static constexpr const IdTypes  PARENTS = Atom::ID;

        uint64_t            id = 0ULL;
        constexpr auto    operator<=>(const Place&rhs) const = default; 
        constexpr operator bool() const { return id; }
    };
}

YQ_TYPE_DECLARE(yq::mithril::Place)
YQ_TYPE_DECLARE(yq::mithril::PlaceSet)
YQ_TYPE_DECLARE(yq::mithril::PlaceVector)
