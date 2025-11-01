////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/document/Document.hpp>

namespace yq::mithril {

    /*! \brief Atom class in the cache

        This structure represents an atom classification in the cache
    */
    struct Category {
        static constexpr const IdTypeId ID          = 5;
        static constexpr const IdTypes  PARENTS     = Document::ID;
        static constexpr const char*    EXTENSION   = "cat";
        struct Info;
        struct File;
        struct Data;
        struct Diff;
        
        using SharedData = std::shared_ptr<Data>;
        using SharedFile = std::shared_ptr<File>;
        
        using Notify    = Notifier<const Diff&>;
        
        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Category&rhs) const noexcept = default;
        constexpr operator uint64_t() const noexcept { return id; }
        constexpr bool valid() const { return static_cast<bool>(id); }
    };
}

YQ_TYPE_DECLARE(yq::mithril::Category)
YQ_TYPE_DECLARE(yq::mithril::CategorySet)
YQ_TYPE_DECLARE(yq::mithril::CategoryVector)
