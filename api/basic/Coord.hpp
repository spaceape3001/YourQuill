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
    
    template <typename T, uint8_t N>
    constexpr Coord<T,N>  operator+(const Coord<T,N>& a) noexcept
    {
        return a;
    }
    
    template <typename T, uint8_t N>
    consteval uint8_t dims(const Coord<T,N>&) noexcept
    {
        return N;
    }
}
