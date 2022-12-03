////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/preamble.hpp>

namespace yq {
    template <typename> class IDLock;

    /*! User in the cache database
    */
    struct User {
        static constexpr const char*    szExtension = "user";
        struct Info;
        struct Data;
        class File;
        using SharedData = std::shared_ptr<Data>;
        using SharedFile = std::shared_ptr<File>;
        using Lock = IDLock<User>;
        
        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const User&rhs) const noexcept = default;
        constexpr operator uint64_t() const noexcept { return id; }
    };
}

YQ_TYPE_DECLARE(yq::User)
