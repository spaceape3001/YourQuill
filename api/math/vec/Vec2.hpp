////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

//  WARNING!  WARNING!
//
//  THIS FILE IS AUTO-GENERATED, CHANGES WILL BE CLOBBERED!

#define YQ__API__MATH__VECTOR_2__HPP 1
#include <math/preamble.hpp>

namespace yq {
    /*! \brief Vector of 2 dimension(s)
    
        This is a 2 dimensional cartesian vector of the given type.
        \tparam[T]  Data type to be used, recommended to be arithmetic in nature
    */
    template <typename T>
    struct Vec2 {
        //! Component data type argument to this structure (ie, template parameter T)
        using component_type = T;

        /*! \brief Creates a unit-vector in the $(bit)s dimension.
        */
        static consteval Vec2 unit_x();
        /*! \brief Creates a unit-vector in the $(bit)s dimension.
        */
        static consteval Vec2 unit_y();

        //! Component for the $(bit)s dimension.
        T       x;
        //! Component for the $(bit)s dimension.
        T       y;;
        
        //! Equality operator (using default)
        constexpr bool operator==(const Vec2&) const noexcept = default;
    };
    
//  --------------------------------------------------------
//  COMPOSITION

    /*! \brief Creates a $(N)d dimensioal vector
    
        Helper function to create a $(N) dimensional cartesian vector where the component type is deduced from
        the first argument.
    */
    template <typename T>
    constexpr Vec2<T> vec(T x, std::type_identity_t<T> y)
    {
        return {x,y};
    }
    
    template <typename T>
    consteval Vec2<T> Vec2<T>::unit_x()
    {
        return {one_v<T>,zero_v<T>};
    }

    template <typename T>
    consteval Vec2<T> Vec2<T>::unit_y()
    {
        return {zero_v<T>,one_v<T>};
    }

    YQ_NAN_1(Vec2, Vec2<T>{nan_v<T>, nan_v<T>})
    YQ_ZERO_1(Vec2, Vec2<T>{zero_v<T>, zero_v<T>})
    
//  --------------------------------------------------------
//  BASIC FUNCTIONS

    YQ_IS_NAN_1(Vec2, is_nan(v.x) || is_nan(v.y))
    YQ_IS_FINITE_1(Vec2, is_finite(v.x) && is_finite(v.y))
        
    /*! \brief Square of the vector's length
    
        This returns the SQUARE of the given vector's length.
    */
    template <typename T>
    constexpr square_t<T> length2(const Vec2<T>& a)
    {
        return a.x*a.x + a.y*a.y;
    }    
    
    /*! \brief Length of the vector
        
        This returns the length of the given vector.
    */
    template <typename T>
    requires trait::has_sqrt_v<T>
    auto    length(const Vec2<T>& a)
    {
        return sqrt(length2(a));
    }

//  --------------------------------------------------------
//  POSITIVE

    template <typename T>
    constexpr Vec2<T> operator+(const Vec2<T>& a)
    {
        return a;
    }

//  --------------------------------------------------------
//  NEGATIVE

    template <typename T>
    constexpr Vec2<T> operator-(const Vec2<T>&a)
    {
        return {-a.x,-a.y};
    }

//  --------------------------------------------------------
//  NORMALIZATION

    template <typename T>
    requires trait::has_sqrt_v<T>
    Vec2<quotient_t<T,T>> operator~(const Vec2<T>& a)
    {
        auto l = one_v<T>/length(a);
        return {a.x/l, a.y/l};
    }

//  --------------------------------------------------------
//  ADDITION

    template <typename T>
    constexpr Vec2<T> operator+(const Vec2<T>& a, const Vec2<T>& b)
    {
        return {a.x+b.x, a.y+b.y};
    }
    
    template <typename T>
    Vec2<T>& operator+=(Vec2<T>& a, const Vec2<T>& b)
    {
        a.x += b.x;
        a.y += b.y;
        return a;
    }

//  --------------------------------------------------------
//  SUBTRACTION

    template <typename T>
    constexpr Vec2<T> operator-(const Vec2<T>& a, const Vec2<T>& b)
    {
        return {a.x-b.x, a.y-b.y};
    }
    
    template <typename T>
    Vec2<T>& operator-=(Vec2<T>& a, const Vec2<T>& b)
    {
        a.x -= b.x;
        a.y -= b.y;
        return a;
    }


//  --------------------------------------------------------
//  MULTIPLICATION

    template <typename T, typename U>
    requires (std::is_arithmetic_v<T>)
    constexpr Vec2<product_t<T,U>> operator*(T a, const Vec2<U>&b)
    {
        return {a*b.x, a*b.y};
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U>)
    constexpr Vec2<product_t<T,U>> operator*(const Vec2<T>& a, U b)
    {
        return {a.x*b, a.y*b};
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_mul_v<T,U>)
    Vec2<T>& operator*=(Vec2<T>& a, T b)
    {
        a.y *= b;
        return a;
    }

//  --------------------------------------------------------
//  DIVISION

    template <typename T, typename U>
    requires (std::is_arithmetic_v<T>)
    constexpr  Vec2<quotient_t<T,U>> operator/(T a, const  Vec2<U>&b)
    {
        return (a*b) / length2(b);
    }

    template <typename T, typename U>
    requires (std::is_arithmetic_v<U>)
    constexpr  Vec2<quotient_t<T,U>> operator/(const  Vec2<T>& a, U b)
    {
        return {a.x / b, a.y / b};
    }

    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_div_v<T,U>)
    Vec2<T>& operator/=(Vec2<T>& a, U b)
    {
        a.x /= b;
        a.y /= b;
        return a;
    }


//  --------------------------------------------------------
//  DOT PRODUCT

    template <typename T, typename U>
    constexpr product_t<T,U> operator DOT (const Vec2<T>& a, const Vec2<U>&b)
    {
        return a.x*b.x + a.y*b.y;
    }

//  --------------------------------------------------------
//  INNER PRODUCT

    template <typename T, typename U>
    constexpr product_t<T,U> operator INNER (const Vec2<T>& a, const Vec2<U>&b)
    {
        return a.x*b.x + a.y*b.y;
    }
}

YQ_TYPE_DECLARE(yq::Vec2D)
YQ_TYPE_DECLARE(yq::Vec2F)
YQ_TYPE_DECLARE(yq::Vec2I)
YQ_TYPE_DECLARE(yq::Vec2U)

