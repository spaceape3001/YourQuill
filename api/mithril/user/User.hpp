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
        static constexpr const IdTypeId ID          = 27;
        static constexpr const IdTypes  PARENTS     = Document::ID;

        static constexpr const char*    EXTENSION = "user";
        struct Info;
        struct Data;
        struct Diff;
        class File;
        using SharedData = std::shared_ptr<Data>;
        using SharedFile = std::shared_ptr<File>;
        
        using Notify    = Notifier<const Diff&>;
        
        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const User&rhs) const noexcept = default;
        constexpr operator uint64_t() const noexcept { return id; }
    };

    using UserFragDoc  = std::pair<Fragment, User::SharedFile>;
}

YQ_TYPE_DECLARE(yq::mithril::User)
YQ_TYPE_DECLARE(yq::mithril::UserSet)
YQ_TYPE_DECLARE(yq::mithril::UserVector)
