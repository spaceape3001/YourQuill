////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/id/Id.hpp>

namespace yq::mithril {
    /*! \brief Predefined value for a field
    
        This is a predefined attribute value for a field
    */
    struct Value {
        static constexpr const IdTypeId  ID        = 29;
        static constexpr const IdTypes    PARENTS    = {};

        struct Data;
        struct Info;
        struct ListData;
        struct ListFile;

        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Value&rhs) const noexcept = default;
        constexpr operator uint64_t () const noexcept { return id; }
    };
}

YQ_TYPE_DECLARE(yq::mithril::Value)
YQ_TYPE_DECLARE(yq::mithril::ValueSet)
YQ_TYPE_DECLARE(yq::mithril::ValueVector)
