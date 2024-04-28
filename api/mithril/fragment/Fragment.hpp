////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/id/Id.hpp>

namespace yq::mithril {

    /*! \brief Represents single fragment
    */
    struct Fragment {
        static constexpr const IdTypeId  ID      = 15;
        static constexpr const IdTypes   PARENTS = {};

        struct Info;
        uint64_t  id = 0ULL;
        constexpr auto    operator<=>(const Fragment& rhs) const noexcept = default;
        constexpr operator uint64_t() const noexcept { return id != 0ULL; }
    };
}

YQ_TYPE_DECLARE(yq::mithril::Fragment)
YQ_TYPE_DECLARE(yq::mithril::FragmentSet)
YQ_TYPE_DECLARE(yq::mithril::FragmentVector)
