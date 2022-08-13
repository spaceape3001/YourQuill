////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <compare>

namespace yq {


    /*! \brief Folder of the cache/workspace

        Folder represents a fusion of the same named directory across the roots of the worksapce (case insensitive).
    */
    struct Folder {
        struct Info;
        uint64_t  id = 0ULL;
        constexpr auto    operator<=>(const Folder&rhs) const = default;
        static constexpr const uint64_t TOP         = 1ULL;
        static constexpr const uint64_t CONFIG      = 2ULL;
        static constexpr const uint64_t CATEGORIES  = 3ULL;
        static constexpr const uint64_t CLASSES     = 4ULL;
        static constexpr const uint64_t FIELDS      = 5ULL;
        static constexpr const uint64_t TAGS        = 6ULL;
        static constexpr const uint64_t USERS       = 7ULL;
        constexpr operator bool() const { return id != 0ULL; }
    };

    namespace cdb {
        static constexpr const uint64_t     kMaxFixedFolder = Folder::USERS;
    }
}
