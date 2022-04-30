#pragma once

namespace yq {
    /*! \brief Attribute of documents
    */
    struct Attribute {
        uint64_t    id  = 0ULL;
        constexpr auto    operator<=>(const Attribute&rhs) const = default; 
        constexpr operator bool() const { return id != 0ULL; }

    };
}
