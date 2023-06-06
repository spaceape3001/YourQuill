////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <mithril/preamble.hpp>

namespace yq::mithril {
    /*! \brief Book in the cache database */
    struct Book {
        uint64_t            id = 0ULL;
        using Lock = IDLock<Book>;
        constexpr auto    operator<=>(const Book&rhs) const = default; 
        constexpr operator bool() const { return id != 0ULL; }
    };
}

YQ_TYPE_DECLARE(yq::mithril::Book)
