////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <compare>
#include <cstdint>


namespace yq {

    /*! \brief Attribute of documents
    */
    struct Attribute {
        struct Info;
        struct Diff;
        struct Report;
        struct KVUA;

        uint64_t    id  = 0ULL;
        constexpr auto    operator<=>(const Attribute&rhs) const = default; 
        constexpr operator bool() const { return id != 0ULL; }
    };
}
