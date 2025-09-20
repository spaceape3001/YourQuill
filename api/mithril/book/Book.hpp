////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/Atom.hpp>

namespace yq::mithril {
    /*! \brief Book in the cache database */
    struct Book {
        static constexpr const IdTypeId ID      = 4;
        static constexpr const IdTypes  PARENTS = Atom::ID;

        uint64_t            id = 0ULL;
        constexpr auto    operator<=>(const Book&rhs) const = default; 
        constexpr operator uint64_t() const { return id; }
        constexpr bool valid() const { return static_cast<bool>(id); }
    };
}

YQ_TYPE_DECLARE(yq::mithril::Book)
YQ_TYPE_DECLARE(yq::mithril::BookSet)
YQ_TYPE_DECLARE(yq::mithril::BookVector)
