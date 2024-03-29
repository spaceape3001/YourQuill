////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/entity/Entity.hpp>
#include <mithril/group/Group.hpp>

namespace yq::mithril {
    /*! \brief Organization (of entities/etc) in the cache database */
    struct Organization {
        static constexpr const IdTypeId ID      = 20;
        static constexpr const IdTypes  PARENTS = { Entity::ID, Group::ID};

        uint64_t            id = 0ULL;
        constexpr auto    operator<=>(const Organization&rhs) const = default; 
        constexpr operator uint64_t() const { return id; }
    };
}
YQ_TYPE_DECLARE(yq::mithril::Organization)
