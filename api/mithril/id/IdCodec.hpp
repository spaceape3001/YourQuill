////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/id/IdType.hpp>

namespace yq::mithril {
    static constexpr    uint64_t    ID_MASK         = 0x00FF'FFFF'FFFF'FFFFULL;
    static constexpr    uint64_t    ID_TYPE_SHIFT   = 56ULL;
    
    constexpr uint64_t          id_encode(IdType type, uint64_t id) noexcept
    {
        return (((uint64_t) type) << ID_TYPE_SHIFT) | (id & ID_MASK);
    }
    
    template <cdb_object S>
    constexpr uint64_t          id_encode(S s) noexcept
    {
        return encode(id_type_v<S>, s.id);
    }
    
    constexpr IdType            id_type_of(uint64_t v) noexcept
    {
        return (IdType) (v >> ID_TYPE_SHIFT);
    }
    
    constexpr uint64_t          id_of(uint64_t v) noexcept
    {
        return v & ID_MASK;
    }
    
    template <cdb_object S>
    constexpr S                 id_decode(uint64_t v) noexcept
    {
        return id_compatible(id_type_of(v), id_type_v<S>) ? S(id_of(v)) : S();
    }
    
    constexpr bool              id_valid(uint64_t v) noexcept
    {
        return v && (id_type_of(v) != IdType::Unknown);
    }
}
