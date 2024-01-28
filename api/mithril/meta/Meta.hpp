////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/document/Document.hpp>

namespace yq::mithril {
    

    /*! \brief Meta ID
        
        This allows for an id of the metatype
    */
    struct Meta {
        static constexpr const IdTypeId ID          = 20;
        static constexpr const IdTypes  PARENTS     = {};

        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Meta&rhs) const noexcept = default;
        constexpr operator uint64_t() const noexcept { return id; }
    };
}

YQ_TYPE_DECLARE(yq::mithril::Meta)
