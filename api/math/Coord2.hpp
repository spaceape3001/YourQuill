////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math/preamble.hpp>

namespace yq {
    template <typename T>
    struct Coord2 {
    
        static_assert(std::is_arithmetic_v<T>, "Template parameter T must be arithmetic type.");

        T      i = zero_v<T>;
        T      j = zero_v<T>;

        constexpr Coord2() {}
        explicit constexpr Coord2(T d) : i(d), j(d) {}
        constexpr Coord2(T _i, T _j) : i(_i), j(_j) {}

        //  todo...
            // Coord2(const Vec2I&);
        constexpr bool operator==(const Coord2&) const noexcept = default;
    };
    
    YQ_NAN_1(Coord2, Coord2<T>{ nan_v<T>, nan_v<T> })
    YQ_IS_NAN_1(Coord2, is_nan(v.i) || is_nan(v.j) )
    YQ_IS_FINITE_1(Coord2, is_finite(v.i) && is_finite(v.j) )
    YQ_ZERO_1(Coord2, Coord2<T>{ zero_v<T>, zero_v<T> })
    
    
    template <typename T>
    constexpr Coord2<T> operator+(const Coord2<T>&a)
    {
        return a;
    }
    
    template <typename T>
    constexpr Coord2<T> operator-(const Coord2<T>&a)
    {
        return { -a.i, -a.j };
    }

    template <typename T>
    constexpr Coord2<T> operator+(const Coord2<T>& a, const Coord2<T>& b)
    {
        return { a.i+b.i, a.j+b.j };
    }
    
    template <typename T>
    Coord2<T>& operator+=(Coord2<T>& a, const Coord2<T>&b)
    {
        a.i += b.i;
        a.j += b.j;
        return a;
    }

    template <typename T>
    constexpr Coord2<T> operator-(const Coord2<T>& a, const Coord2<T>& b)
    {
        return { a.i-b.i, a.j-b.j };
    }

    template <typename T>
    Coord2<T>& operator-=(Coord2<T>& a, const Coord2<T>&b)
    {
        a.i -= b.i;
        a.j -= b.j;
        return a;
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<T>)
    constexpr Coord2<decltype(T()*U())> operator*(T a, const Coord2<U>&b)
    {
        return { a * b.i, a * b.j };
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U>)
    constexpr Coord2<decltype(T()*U())> operator*(const Coord2<T>& a, U b)
    {
        return { a.i * b,  a.j * b };
    }

    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && std::is_same_v<T, decltype(T()*U())>)
    Coord2<T>& operator*=(Coord2<T>& a, U b)
    {
        a.i *= b;
        a.j *= b;
        return a;
    }

    template <typename T, typename U>
    constexpr Coord2<decltype(T()*U())> operator*(const Coord2<T>& a, const Coord2<U>& b)
    {
        return { a.i*b.i, a.j*b.j };
    }
    
    template <typename T, typename U>
    requires (std::is_same_v<T, decltype(T()*U())>)
    Coord2<T>& operator*=(Coord2<T>&a, const Coord2<U>& b)
    {
        a.i *= b.i;
        a.j *= b.j;
        return a;
    }
    

    template <typename T, typename U>
    requires (std::is_arithmetic_v<U>)
    constexpr Coord2<decltype(T()*U())> operator/(const Coord2<T>& a, U b)
    {
        return { a.i / b,  a.j / b };
    }

    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && std::is_same_v<T, decltype(T()/U())>)
    Coord2<T>& operator/=(Coord2<T>& a, U b)
    {
        a.i /= b;
        a.j /= b;
        return a;
    }

    template <typename T, typename U>
    constexpr Coord2<decltype(T()*U())> operator/(const Coord2<T>& a, const Coord2<U>& b)
    {
        return { a.i/b.i, a.j/b.j };
    }

    template <typename T, typename U>
    requires (std::is_same_v<T, decltype(T()/U())>)
    Coord2<T>& operator/=(Coord2<T>& a, const Coord2<U>& b)
    {
        a.i /= b.i;
        a.j /= b.j;
        return a;
    }
    
    template <typename S, typename I>
    S&  as_stream(S& s, const Coord2<I>& v)
    {
        return s << "[" << v.i << "," << v.j << "]";
    }
    
    template <typename T>
    Stream& operator<<(Stream&s, const Coord2<T>& v)
    {
        return as_stream(s, v);
    }

    template <typename T>
    log4cpp::CategoryStream& operator<<(log4cpp::CategoryStream& s, const Coord2<T>& v)
    {
        return as_stream(s, v);
    }
}

YQ_TYPE_DECLARE(yq::Coord2D)
YQ_TYPE_DECLARE(yq::Coord2F)
YQ_TYPE_DECLARE(yq::Coord2I)
YQ_TYPE_DECLARE(yq::Coord2U)

