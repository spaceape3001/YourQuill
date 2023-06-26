////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/id/Id.hpp>

namespace yq::mithril {
    /*! \brief Attribute of documents
    */
    struct Attribute {
        static constexpr const IdTypeId ID      = 3;
        static constexpr const IdTypes  PARENTS = {};
        struct Info;
        struct Diff;
        struct Report;
        struct KVUA;

        uint64_t    id  = 0ULL;
        constexpr auto    operator<=>(const Attribute&rhs) const noexcept = default; 
        constexpr operator uint64_t() const noexcept { return id; }
    };
}

YQ_TYPE_DECLARE(yq::mithril::Attribute)
