////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <compare>


namespace yq {

    template <typename> class IDLock;

    /*! \brief Represents single fragment
    */
    struct Fragment {
        struct Info;
        using Lock = IDLock<Fragment>;
        uint64_t  id = 0ULL;
        constexpr auto    operator<=>(const Fragment& rhs) const = default;
        constexpr operator bool() const { return id != 0ULL; }
    };
}
