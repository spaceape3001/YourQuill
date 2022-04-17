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
    /*! Tag in the cache database
    */
    struct Tag {
        static constexpr const char*    szExtension = "tag";
        struct Info;
        struct Data;
        class File;
        using SharedData = std::shared_ptr<Data>;
        using SharedFile = std::shared_ptr<File>;
        
        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Tag&rhs) const = default;
        constexpr operator bool() const { return id != 0ULL; }
    };
    
}
