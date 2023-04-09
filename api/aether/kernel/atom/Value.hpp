////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <aether/kernel/preamble.hpp>

namespace yq {
    /*! \brief Predefined value for a field
    
        This is a predefined attribute value for a field
    */
    struct Value {
        struct Data;
        struct Info;
        struct ListData;
        struct ListFile;

        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Value&rhs) const noexcept = default;
        constexpr operator uint64_t () const noexcept { return id; }
    };
}

YQ_TYPE_DECLARE(yq::Value)
