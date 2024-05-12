////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/document/Document.hpp>

namespace yq::mithril {
    struct Novel {
        static constexpr const IdTypeId ID          = 22;
        static constexpr const IdTypes  PARENTS     = Document::ID;
        static constexpr const char*    EXTENSION   = "nw";
        struct Info;
        
        struct Diff;

        using Notify = Notifier<const Diff&>;
        
        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Novel&rhs) const noexcept = default;
        constexpr operator uint64_t () const noexcept { return id; }
    };
}

YQ_TYPE_DECLARE(yq::mithril::Novel)
YQ_TYPE_DECLARE(yq::mithril::NovelSet)
YQ_TYPE_DECLARE(yq::mithril::NovelVector)
