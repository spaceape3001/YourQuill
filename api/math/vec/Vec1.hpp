////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

//  WARNING!  WARNING!
//
//  THIS FILE IS AUTO-GENERATED, CHANGES WILL BE CLOBBERED!

#define YQ__API__MATH__VECTOR_1__HPP 1
#include <math/preamble.hpp>

namespace yq {
    /*! \brief Vector of 1 dimension(s)
    
        This is a 1 dimensional cartesian vector of the given type.
        \tparam[T]  Data type to be used, recommended to be arithmetic in nature
    */
    template <typename T>
    struct Vec1 {
        //! Component data type argument to this structure (ie, template parameter T)
        using component_type = T;

        /*! \brief Creates a unit-vector in the $(bit)s dimension.
        */
        static consteval Vec1 unit_x();

        //! Component for the $(bit)s dimension.
        T       x;;
        
        //! Equality operator (using default)
        constexpr bool operator==(const Vec1&) const noexcept = default;
    };
    
//  --------------------------------------------------------
//  COMPOSITION

    /*! \brief Creates a $(N)d dimensioal vector
    
        Helper function to create a $(N) dimensional cartesian vector where the component type is deduced from
        the first argument.
    */
    template <typename T>
    constexpr Vec1<T> vec(T x)
    {
        return {x};
    }
    
    template <typename T>
    consteval Vec1<T> Vec1<T>::unit_x()
    {
        return {one_v<T>};
    }

    YQ_NAN_1(Vec1, Vec1<T>{nan_v<T>})
    YQ_ZERO_1(Vec1, Vec1<T>{zero_v<T>})
    
//  --------------------------------------------------------
//  BASIC FUNCTIONS

    YQ_IS_NAN_1(Vec1, is_nan(v.x))
    YQ_IS_FINITE_1(Vec1, is_finite(v.x))
        
    /*! \brief Square of the vector's length
    
        This returns the SQUARE of the given vector's length.
    */
    template <typename T>
    constexpr square_t<T> length2(const Vec1<T>& a)
    {
        return a.x*a.x;
    }    
    
    /*! \brief Length of the vector
        
        This returns the length of the given vector.
    */
    template <typename T>
    requires trait::has_abs_v<T>
    auto    length(const Vec1<T>& a)
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
        return {-a.x};
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
        return {a.x+b.x};
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
    constexpr Vec1<T> operator-(const Vec1<T>& a, const Vec1<T>& b)
    {
        return {a.x-b.x};
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
    constexpr Vec1<product_t<T,U>> operator*(T a, const Vec1<U>&b)
    {
        return {a*b.x};
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U>)
    constexpr Vec1<product_t<T,U>> operator*(const Vec1<T>& a, U b)
    {
        return {a.x*b};
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_mul_v<T,U>)
    Vec1<T>& operator*=(Vec1<T>& a, T b)
    {
        a.x *= b;
        return a;
    }

    template <typename T, typename U>
    product_t<T,U> operator*(const Vec1<T>&a, const Vec1<U>& b)
    {
        return a.x*b.x;
    }

//  --------------------------------------------------------
//  DIVISION

    template <typename T, typename U>
    requires (std::is_arithmetic_v<T>)
    constexpr  Vec1<quotient_t<T,U>> operator/(T a, const  Vec1<U>&b)
    {
        return (a*b) / length2(b);
    }

    template <typename T, typename U>
    requires (std::is_arithmetic_v<U>)
    constexpr  Vec1<quotient_t<T,U>> operator/(const  Vec1<T>& a, U b)
    {
        return {a.x / b};
    }

    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_div_v<T,U>)
    Vec1<T>& operator/=(Vec1<T>& a, U b)
    {
        a.x /= b;
        return a;
    }

    template <typename T, typename U>
    quotient_t<T,U> operator/(const Vec1<T>&a, const Vec1<U>& b)
    {
        return (a*b)/length2(b);
    }


//  --------------------------------------------------------
//  DOT PRODUCT

    template <typename T, typename U>
    constexpr product_t<T,U> operator DOT (const Vec1<T>& a, const Vec1<U>&b)
    {
        return a.x*b.x;
    }

//  --------------------------------------------------------
//  INNER PRODUCT

    template <typename T, typename U>
    constexpr product_t<T,U> operator INNER (const Vec1<T>& a, const Vec1<U>&b)
    {
        return a.x*b.x;
    }

    //  --------------------------------------------------------
    //  ADVANCED FUNCTIONS

    //! TRUE if every component of a is less than b
    template <typename T>
    bool        all_less(const Vec1<T>& a, const Vec1<T>&b)
    {
        return (a.x<b.x);
    }

    //! TRUE if every component of a is less than (or equal to) b
    template <typename T>
    bool        all_less_equal(const Vec1<T>& a, const Vec1<T>&b)
    {
        return (a.x<=b.x);
    }

    //! TRUE if every component of a is greater than b
    template <typename T>
    bool        all_greater(const Vec1<T>& a, const Vec1<T>&b)
    {
        return (a.x>b.x);
    }

    //! TRUE if every component of a is greater or equal to b
    template <typename T>
    bool        all_greater_equal(const Vec1<T>& a, const Vec1<T>&b)
    {
        return (a.x>=b.x);
    }
}

YQ_TYPE_DECLARE(yq::Vec1D)
YQ_TYPE_DECLARE(yq::Vec1F)
YQ_TYPE_DECLARE(yq::Vec1I)
YQ_TYPE_DECLARE(yq::Vec1U)

