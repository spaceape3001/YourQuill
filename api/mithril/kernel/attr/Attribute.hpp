////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/kernel/preamble.hpp>

namespace yq {

    /*! \brief Attribute of documents
    */
    struct Attribute {
        struct Info;
        struct Diff;
        struct Report;
        struct KVUA;

        uint64_t    id  = 0ULL;
        constexpr auto    operator<=>(const Attribute&rhs) const noexcept = default; 
        constexpr operator uint64_t() const noexcept { return id; }
    };
}

YQ_TYPE_DECLARE(yq::Attribute)
