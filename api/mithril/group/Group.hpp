////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <mithril/atom/Atom.hpp>

namespace yq::mithril {
    /*! \brief Group (of entities/etc) in the cache database */
    struct Group {
        static constexpr const IdTypeId ID      = 17;
        static constexpr const IdTypes  PARENTS = Atom::ID;

        uint64_t            id = 0ULL;
        constexpr auto    operator<=>(const Group&rhs) const = default; 
        constexpr operator uint64_t() const { return id; }
    };
}

YQ_TYPE_DECLARE(yq::mithril::Group)
YQ_TYPE_DECLARE(yq::mithril::GroupSet)
YQ_TYPE_DECLARE(yq::mithril::GroupVector)
