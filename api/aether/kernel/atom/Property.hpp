////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/preamble.hpp>

namespace yq {
    /*! \brief Predefined value for a field
    
        This is a predefined attribute value for a field
    */
    struct Property {

        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Property&rhs) const noexcept = default;
        constexpr operator uint64_t () const noexcept { return id; }
    };
}

YQ_TYPE_DECLARE(yq::Property)
