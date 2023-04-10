////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/kernel/preamble.hpp>

namespace yq::mithril {

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
        constexpr auto    operator<=>(const Category&rhs) const noexcept = default;
        constexpr operator uint64_t() const noexcept { return id; }
    };
}

YQ_TYPE_DECLARE(yq::mithril::Category)
