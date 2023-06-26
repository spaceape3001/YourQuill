////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/document/Document.hpp>

namespace yq::mithril {
    /*! User in the cache database
    */
    struct User {
        static constexpr const IdTypeId ID          = 25;
        static constexpr const IdTypes  PARENTS     = Document::ID;

        static constexpr const char*    EXTENSION = "user";
        struct Info;
        struct Data;
        class File;
        using SharedData = std::shared_ptr<Data>;
        using SharedFile = std::shared_ptr<File>;
        
        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const User&rhs) const noexcept = default;
        constexpr operator uint64_t() const noexcept { return id; }
    };
}

YQ_TYPE_DECLARE(yq::mithril::User)
