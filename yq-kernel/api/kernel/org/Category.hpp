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

    /*! \brief Atom class in the cache

        This structure represents an atom classification in the cache
    */
    struct Category {
        static constexpr const char*    szExtension = "cat";
        struct Info;
        struct File;
        struct Data;
        using Lock = IDLock<Category>;
        using SharedData = std::shared_ptr<Data>;
        using SharedFile = std::shared_ptr<File>;
        
        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Category&rhs) const = default;
        constexpr operator bool() const { return id != 0ULL; }
    };
}
