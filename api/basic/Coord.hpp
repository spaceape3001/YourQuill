////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "preamble.hpp"
#include <cassert>
#include <type_traits>

namespace yq {
    template <typename T>
    struct Coord<T,1> {
        using component_type    = T;
        static constexpr const uint8_t  DIMS    = 1;
        T       i;
        constexpr bool operator==(const Coord&) const noexcept = default;
    };

    template <typename T>
    struct Coord<T,2> {
        using component_type    = T;
        static constexpr const uint8_t  DIMS    = 2;
        T       i, j;
        constexpr bool operator==(const Coord&) const noexcept = default;
    };

    template <typename T>
    struct Coord<T,3> {
        using component_type    = T;
        static constexpr const uint8_t  DIMS    = 3;
        T       i, j, k;
        constexpr bool operator==(const Coord&) const noexcept = default;
    };

    template <typename T>
    struct Coord<T,4> {
        using component_type    = T;
        static constexpr const uint8_t  DIMS    = 4;
        T       i, j, k, l;
        constexpr bool operator==(const Coord&) const noexcept = default;
    };

    template <typename T>
    struct Coord<T,5> {
        using component_type    = T;
        static constexpr const uint8_t  DIMS    = 5;
        T       i, j, k, l, m;
        constexpr bool operator==(const Coord&) const noexcept = default;
    };

    template <typename T>
    struct Coord<T,6> {
        using component_type    = T;
        static constexpr const uint8_t  DIMS    = 6;
        T       i, j, k, l, m, n;
        constexpr bool operator==(const Coord&) const noexcept = default;
    };
    
    
    template <typename T, uint8_t N>
    constexpr T   get(const Coord<T,N>& c, uint8_t i)
    {
        assert(i<N);
        return ((const T*)&c)[i];
    }

    template <typename T, uint8_t N>
    constexpr T&  edit(Coord<T,N>& c, uint8_t i)
    {
        assert(i<N);
        return ((const T*)&c)[i];
    }

    template <typename T, uint8_t N>
    void set (Coord<T,N>& c, uint8_t i, T v)
    {
        assert(i<N);
        edit(c,i) = v;
    }

    namespace trait {
        template <typename> struct is_coord : std::false_type {};
        template <typename T, uint8_t N>
        struct is_coord<Coord<T,N>> : std::true_type {};
        
        template <typename T>
        inline constexpr bool is_coord_v = is_coord<T>::value;
    }
}
