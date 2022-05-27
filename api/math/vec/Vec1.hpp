////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math/preamble.hpp>

namespace yq {

    template <typename T>
    struct Vec1 {
        using component_type = T;
        T       x;
        bool   operator==(const Vec1& b) const noexcept = default;
        static consteval Vec1 unit_x();
    };

//  --------------------------------------------------------
//  BASIC FUNCTIONS


    YQ_NAN_1(Vec1, Vec1<T>{ nan_v<T> })
    YQ_IS_NAN_1(Vec1, is_nan(v.x))
    YQ_IS_FINITE_1(Vec1, is_finite(v.x))
    YQ_ZERO_1(Vec1, Vec1<T>{ zero_v<T> })

    template <typename T>
    consteval Vec1<T> Vec1<T>::unit_x()
    {
        return { one_v<T> };
    }
    
    template <typename T>
    auto      length2(const Vec1D& a)
    {
        return a.x*a.x;
    }
    
    template <typename T>
    requires trait::has_abs_v<T>
    auto    length(const Vec1D& a)
    {
        return abs(a.x);
    }
    
//  --------------------------------------------------------
//  POSITIVE

    template <typename T>
    constexpr Vec1<T> operator+(const Vec1<T>& a)
    {
        return a;
    }
    
//  --------------------------------------------------------
//  NEGATIVE

    template <typename T>
    constexpr Vec1<T> operator-(const Vec1<T>&a)
    {
        return { -a.x };
    }

//  --------------------------------------------------------
//  NORMALIZATION

    template <typename T>
    requires trait::has_copysign_v<T>
    Vec1<T> operator~(const Vec1<T>& a)
    {
        return Vec1<T>(copysign(one_v<T>, a.x));
    }

//  --------------------------------------------------------
//  ADDITION

    template <typename T>
    constexpr Vec1<T> operator+(const Vec1<T>& a, const Vec1<T>& b)
    {
        return { a.x + b.x };
    }
    
    template <typename T>
    Vec1<T>& operator+=(Vec1<T>& a, const Vec1<T>& b)
    {
        a.x += b.x;
        return a;
    }

//  --------------------------------------------------------
//  SUBTRACTION
    
    template <typename T>
    constexpr Vec1<T> operator-(const Vec1<T>&a, const Vec1<T>& b)
    {
        return { a.x - b.x };
    }
    
    template <typename T>
    Vec1<T>& operator-=(Vec1<T>& a, const Vec1<T>& b)
    {
        a.x -= b.x;
        return a;
    }
    
//  --------------------------------------------------------
//  MULTIPLICATION


    template <typename T, typename U>
    requires (std::is_arithmetic_v<T>)
    constexpr Vec1<decltype(T()*U())> operator*(T a, const Vec1<T>&b)
    {
        return { a * b.x };
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U>)
    constexpr Vec1<decltype(T()*U())> operator*(const Vec1<T>& a, T b)
    {
        return { a.x * b };
    }
    


}

