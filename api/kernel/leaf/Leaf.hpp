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
    template <typename> class IDLock;

    struct Leaf {

        static constexpr const char*    szExtension = "y";
        struct Info;
        struct KV;
        struct Data;
        struct File;
        using SharedData = std::shared_ptr<Data>;
        using SharedFile = std::shared_ptr<File>;
        using Lock = IDLock<Leaf>;
        
        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Leaf&rhs) const = default;
        constexpr operator bool() const { return id != 0ULL; }
    };
}
