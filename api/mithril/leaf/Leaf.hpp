////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>

namespace yq {
    namespace mithril {
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
            constexpr auto    operator<=>(const Leaf&rhs) const noexcept = default;
            constexpr operator uint64_t () const noexcept { return id; }
        };
    }
}

YQ_TYPE_DECLARE(yq::mithril::Leaf)
