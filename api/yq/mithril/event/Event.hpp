////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/atom/Atom.hpp>

namespace yq::mithril {
    /*! \brief Event in the cache database */
    struct Event {
        static constexpr const IdTypeId  ID      = 12;
        static constexpr const IdTypes    PARENTS = Atom::ID;

        uint64_t            id = 0ULL;
        constexpr auto    operator<=>(const Event&rhs) const = default; 
        constexpr operator uint64_t() const { return id; }
        constexpr bool valid() const { return static_cast<bool>(id); }
    };
}

YQ_TYPE_DECLARE(yq::mithril::Event)
YQ_TYPE_DECLARE(yq::mithril::EventSet)
YQ_TYPE_DECLARE(yq::mithril::EventVector)
