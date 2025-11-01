////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/document/Document.hpp>

namespace yq::mithril {
    /*! Tag in the cache database
    */
    struct Tag {
        static constexpr const IdTypeId ID          = 27;
        static constexpr const IdTypes  PARENTS     = Document::ID;
        static constexpr const char*    EXTENSION = "tag";
        
        struct Info;
        struct Data;
        struct Diff;
        
        class File;
        using SharedData = std::shared_ptr<Data>;
        using SharedFile = std::shared_ptr<File>;
        
        
        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Tag&rhs) const noexcept = default;
        constexpr operator uint64_t() const noexcept { return id; }
        constexpr bool valid() const { return static_cast<bool>(id); }
        
        using Notify    = Notifier<const Diff&>;
    };
}

YQ_TYPE_DECLARE(yq::mithril::Tag)
YQ_TYPE_DECLARE(yq::mithril::TagSet)
YQ_TYPE_DECLARE(yq::mithril::TagVector)
