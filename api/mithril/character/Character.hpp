////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/entity/Entity.hpp>

namespace yq::mithril {
    /*! \brief Character in the cache database */
    struct Character {
        static constexpr const IdTypeId ID      = 6;
        static constexpr const IdTypes  PARENTS = Entity::ID;

        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Character&rhs) const = default; 
        constexpr operator uint64_t() const { return id; }
    };
}
YQ_TYPE_DECLARE(yq::mithril::Character)
YQ_TYPE_DECLARE(yq::mithril::CharacterSet)
YQ_TYPE_DECLARE(yq::mithril::CharacterVector)
