////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/document/Document.hpp>

namespace yq::mithril {

    struct Leaf {
        static constexpr const IdTypeId ID          = 20;
        static constexpr const IdTypes  PARENTS     = Document::ID;
        static constexpr const char*    EXTENSION   = "y";
        struct Info;
        struct KV;
        struct Data;
        struct Diff;
        struct File;
        using SharedData = std::shared_ptr<Data>;
        using SharedFile = std::shared_ptr<File>;
        
        using Notify = Notifier<const Diff&>;
        
        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Leaf&rhs) const noexcept = default;
        constexpr operator uint64_t () const noexcept { return id; }
        constexpr bool valid() const { return static_cast<bool>(id); }
    };
}

YQ_TYPE_DECLARE(yq::mithril::Leaf)
YQ_TYPE_DECLARE(yq::mithril::LeafSet)
YQ_TYPE_DECLARE(yq::mithril::LeafVector)
