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
    /*! UserGroup in the cache database
    */
    struct UserGroup {
        static constexpr const char*    szExtension = "ugrp";
        struct Info;
        struct Data;
        class File;
        using SharedData = std::shared_ptr<Data>;
        using SharedFile = std::shared_ptr<File>;
        
        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const UserGroup&rhs) const = default;
        constexpr operator bool() const { return id != 0ULL; }
    };
}
