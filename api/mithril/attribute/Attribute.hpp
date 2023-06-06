////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>

namespace yq::mithril {
    /*! \brief Attribute of documents
    */
    struct Attribute {
        struct Info;
        struct Diff;
        struct Report;
        struct KVUA;
        using Lock = IDLock<Attribute>;

        uint64_t    id  = 0ULL;
        constexpr auto    operator<=>(const Attribute&rhs) const noexcept = default; 
        constexpr operator uint64_t() const noexcept { return id; }
    };
}

YQ_TYPE_DECLARE(yq::mithril::Attribute)
