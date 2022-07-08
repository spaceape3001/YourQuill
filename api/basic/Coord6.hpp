////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Coord.hpp"

namespace yq {

    template <typename T>
    struct Coord<T,6> {
        using component_type    = T;
        static constexpr const uint8_t  DIMS    = 6;
        T       i, j, k, l, m, n;
        constexpr bool operator==(const Coord&) const noexcept = default;
    };


    //  --------------------------------------------------------
    //  COMPOSITION

    template <typename T>
    constexpr Coord6<T>    coord(T i, std::type_identity_t<T> j, std::type_identity_t<T> k, std::type_identity_t<T> l, std::type_identity_t<T> m, std::type_identity_t<T> n) noexcept
    {
        return { i, j, k, l, m, n };
    }
    
    template <typename T>
    constexpr Coord6<T>    coord6(T i)
    {
        return { i, i, i, i, i, i };
    }


    //  --------------------------------------------------------
    //  BASIC FUNCTIONS

    //  --------------------------------------------------------
    //  POSITIVE


    //  --------------------------------------------------------
    //  NEGATIVE

    template <typename T>
    constexpr Coord6<T>    operator-(const Coord6<T>& a)
    {
        return { -a.i, -a.j, -a.k, -a.l, -a.m, -a.n };
    }


    //  --------------------------------------------------------
    //  NORMALIZATION


    //  --------------------------------------------------------
    //  ADDITION
    
    template <typename T>
    constexpr Coord6<T> operator+(const Coord6<T>&a, const Coord6<T>&b)
    {
        return { a.i+b.i, a.j+b.j, a.k+b.k, a.l+b.l, a.m+b.m, a.n+b.n };
    }
    
    template <typename T>
    Coord6<T>&  operator+=(Coord6<T>& a, const Coord6<T>& b)
    {
        a.i += b.i;
        a.j += b.j;
        a.k += b.k;
        a.l += b.l;
        a.m += b.m;
        a.n += b.n;
        return a;
    }


    //  --------------------------------------------------------
    //  SUBTRACTION

    
    template <typename T>
    constexpr Coord6<T> operator-(const Coord6<T>&a, const Coord6<T>&b)
    {
        return { a.i-b.i, a.j-b.j, a.k-b.k, a.l-b.l, a.m-b.m, a.n-b.n };
    }
    
    template <typename T>
    Coord6<T>&  operator-=(Coord6<T>& a, const Coord6<T>& b)
    {
        a.i -= b.i;
        a.j -= b.j;
        a.k -= b.k;
        a.l -= b.l;
        a.m -= b.m;
        a.n -= b.n;
        return a;
    }


    //  --------------------------------------------------------
    //  MULTIPLICATION

    template <typename T, typename U>
    requires (std::is_arithmetic_v<T>)
    constexpr Coord6<decltype(T()*U())> operator*(T a, const Coord6<U>&b)
    {
        return { a*b.i, a*b.j, a*b.k, a*b.l, a*b.m, a*b.n };
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U>)
    constexpr Coord6<decltype(T()*U())> operator*(const Coord6<T>& a, U b)
    {
        return { a.i*b,  a.j*b, a.k*b, a.l*b, a.m*b, a.n*b };
    }

    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && std::is_same_v<T, decltype(T()*U())>)
    Coord6<T>& operator*=(Coord6<T>& a, U b)
    {
        a.i *= b;
        a.j *= b;
        a.k *= b;
        a.l *= b;
        a.m *= b;
        a.n *= b;
        return a;
    }

    template <typename T, typename U>
    constexpr Coord6<decltype(T()*U())> operator*(const Coord6<T>& a, const Coord6<U>& b)
    {
        return { a.i*b.i, a.j*b.j, a.k*b.k, a.l*b.l, a.m*b.m, a.n*b.n };
    }
    
    template <typename T, typename U>
    requires (std::is_same_v<T, decltype(T()*U())>)
    Coord6<T>& operator*=(Coord6<T>&a, const Coord6<U>& b)
    {
        a.i *= b.i;
        a.j *= b.j;
        a.k *= b.k;
        a.l *= b.l;
        a.m *= b.m;
        a.n *= b.n;
        return a;
    }
    
    
    //  --------------------------------------------------------
    //  DIVISION

    template <typename T, typename U>
    requires (std::is_arithmetic_v<U>)
    constexpr Coord6<decltype(T()/U())> operator/(const Coord6<T>& a, U b)
    {
        return { a.i/b,  a.j/b, a.k/b, a.l/b, a.m/b, a.n/b };
    }

    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && std::is_same_v<T, decltype(T()/U())>)
    Coord6<T>& operator/=(Coord6<T>& a, U b)
    {
        a.i /= b;
        a.j /= b;
        a.k /= b;
        a.l /= b;
        a.m /= b;
        a.n /= b;
        return a;
    }

    template <typename T, typename U>
    constexpr Coord6<decltype(T()/U())> operator/(const Coord6<T>& a, const Coord6<U>& b)
    {
        return { a.i/b.i, a.j/b.j, a.k/b.k, a.l/b.l, a.m/b.m, a.n/b.n };
    }

    template <typename T, typename U>
    requires (std::is_same_v<T, decltype(T()/U())>)
    Coord6<T>& operator/=(Coord6<T>& a, const Coord6<U>& b)
    {
        a.i /= b.i;
        a.j /= b.j;
        a.k /= b.k;
        a.l /= b.l;
        a.m /= b.m;
        a.n /= b.n;
        return a;
    }

    //  --------------------------------------------------------
    //  DOT PRODUCT


    //  --------------------------------------------------------
    //  INNER PRODUCT


    //  --------------------------------------------------------
    //  OUTER PRODUCT


    ///  --------------------------------------------------------
    //  OTIMES PRODUCT

    //  --------------------------------------------------------
    //  PROJECTIONS

    //  --------------------------------------------------------
    //  ADVANCED FUNCTIONS


    //  --------------------------------------------------------
    //  CONDITIONAL INCLUDES

    template <typename S, typename I>
    S&  as_stream(S& s, const Coord6<I>& v)
    {
        return s << "[" << v.i << "," << v.j << "," << v.k << "," << v.l << "," << v.m << "," << v.n << "]";
    }
    
    template <typename T>
    Stream& operator<<(Stream&s, const Coord6<T>& v)
    {
        return as_stream(s, v);
    }

    template <typename T>
    log4cpp::CategoryStream& operator<<(log4cpp::CategoryStream& s, const Coord6<T>& v)
    {
        return as_stream(s, v);
    }
    
}
