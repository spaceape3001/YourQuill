////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>

namespace yq {

    /*! \brief Sorted for passing into functions

        Different sort of enum, intended for function calls.
    */
    struct Sorted {
        enum Value : uint8_t { NO = 0, YES };
        Value   value;
        constexpr Sorted() : value(NO) {}
        constexpr Sorted(Value v) : value(v) {}
        operator bool() const { return value != NO; }
    };
}
