////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <mithril/preamble.hpp>

namespace yq::mithril {
    /*! \brief Group (of entities/etc) in the cache database */
    struct Group {
        using Lock = IDLock<Group>;
        uint64_t            id = 0ULL;
        constexpr auto    operator<=>(const Group&rhs) const = default; 
        constexpr operator bool() const { return id != 0ULL; }
    };
}
YQ_TYPE_DECLARE(yq::mithril::Group)
