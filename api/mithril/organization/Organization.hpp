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
        static constexpr const IdTypeId ID      = 23;
        static constexpr const IdTypes  PARENTS = { Entity::ID, Group::ID};

        uint64_t            id = 0ULL;
        constexpr auto    operator<=>(const Organization&rhs) const = default; 
        constexpr operator uint64_t() const { return id; }
        constexpr bool valid() const { return static_cast<bool>(id); }
    };
}
YQ_TYPE_DECLARE(yq::mithril::Organization)
YQ_TYPE_DECLARE(yq::mithril::OrganizationSet)
YQ_TYPE_DECLARE(yq::mithril::OrganizationVector)
