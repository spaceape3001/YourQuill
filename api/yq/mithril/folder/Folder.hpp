////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/id/Id.hpp>

namespace yq::mithril {

    /*! \brief Folder of the cache/workspace

        Folder represents a fusion of the same named directory across the root_dirs of the worksapce (case insensitive).
    */
    struct Folder {
        static constexpr const IdTypeId ID          = 14;
        static constexpr const IdTypes  PARENTS     = {};

        struct Info;
        uint64_t  id = 0ULL;
        constexpr auto    operator<=>(const Folder&rhs) const noexcept = default;
        constexpr operator uint64_t() const noexcept { return id; }
        constexpr bool valid() const { return static_cast<bool>(id); }
        
        static constexpr const uint64_t TOP         = 1ULL;
        static constexpr const uint64_t CONFIG      = 2ULL;
        static constexpr const uint64_t CATEGORIES  = 3ULL;
        static constexpr const uint64_t CLASSES     = 4ULL;
        static constexpr const uint64_t FIELDS      = 5ULL;
        static constexpr const uint64_t STYLES      = 6ULL;
        static constexpr const uint64_t TAGS        = 7ULL;
        static constexpr const uint64_t USERS       = 8ULL;
    };

    namespace cdb {
        static constexpr const uint64_t     kMaxFixedFolder = Folder::USERS;
    }
}

YQ_TYPE_DECLARE(yq::mithril::Folder)
YQ_TYPE_DECLARE(yq::mithril::FolderSet)
YQ_TYPE_DECLARE(yq::mithril::FolderVector)

