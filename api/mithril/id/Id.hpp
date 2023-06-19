////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/id/IdType.hpp>
#include <mithril/id/IdCodec.hpp>
#include <functional>

namespace yq::mithril {
    struct Id {
        uint64_t            value  = 0ULL;
        
        constexpr IdType    type() const noexcept { return id_type_of(value); }
        constexpr uint64_t  id() const noexcept { return id_of(value); }
        constexpr operator uint64_t() const noexcept { return value; }

        constexpr auto operator<=>(const Id&) const noexcept = default;
        
        constexpr Id() noexcept = default;
        template <cdb_object S>
        constexpr Id(S s) noexcept : value(id_encode(s)) {}
        explicit constexpr Id(uint64_t v) noexcept  : value(v) {}
        
        template <cdb_object S>
        constexpr S   as() const noexcept
        {
            return id_decode<S>(value);
        }
    };
    
    using IdProvider    = std::function<std::vector<Id>()>;
    using IdFilter      = std::function<bool(Id)>;
    
}
