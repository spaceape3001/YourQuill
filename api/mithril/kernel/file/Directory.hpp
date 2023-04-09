////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/kernel/preamble.hpp>

namespace yq {

    /*! \brief Directory 

        This represents a directory on the drive, which is reflected in the 
        cache database.
    */
    struct Directory {
        struct Info;
        uint64_t  id = 0ULL;
        constexpr auto    operator<=>(const Directory&rhs) const noexcept = default;
        constexpr operator uint64_t() const noexcept { return id; }
    };
}

YQ_TYPE_DECLARE(yq::Directory)
