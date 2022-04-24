////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <compare>
#include <cstdint>

namespace yq {

    /*! \brief Field of the class

        A field is a pre-declared attribute for any atom using the specified class.  It's recommended to keep fields
        unqiue.
    */
    struct Field {
        struct Info;
        struct Data;
        using SharedData = std::shared_ptr<Data>;

        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Field&rhs) const = default;
        constexpr operator bool() const { return id != 0ULL; }
    };
}
