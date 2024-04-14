////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/id/Id.hpp>

namespace yq::mithril {

    /*! \brief Directory 

        This represents a directory on the drive, which is reflected in the 
        cache database.
    */
    struct Directory {
        static constexpr const IdTypeId    ID      = 8;
        static constexpr const IdTypes      PARENTS = {};

        struct Info;
        uint64_t  id = 0ULL;
        constexpr auto    operator<=>(const Directory&rhs) const noexcept = default;
        constexpr operator uint64_t() const noexcept { return id; }
    };
}

YQ_TYPE_DECLARE(yq::mithril::Directory)
YQ_TYPE_DECLARE(yq::mithril::DirectorySet)
YQ_TYPE_DECLARE(yq::mithril::DirectoryVector)
