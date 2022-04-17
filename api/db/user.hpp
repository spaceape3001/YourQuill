////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <compare>
#include <cstdint>
#include <memory>

namespace yq {
    /*! User in the cache database
    */
    struct User {
        static constexpr const char*    szExtension = "user";
        struct Info;
        struct Data;
        class File;
        using SharedData = std::shared_ptr<Data>;
        using SharedFile = std::shared_ptr<File>;
        
        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const User&rhs) const = default;
        constexpr operator bool() const { return id != 0ULL; }
    };
}
