////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

//  WARNING!  WARNING!
//
//  THIS FILE IS AUTO-GENERATED, CHANGES WILL BE CLOBBERED!

#define YQ__API__MATH__VECTOR_4__HPP 1
#include <math/preamble.hpp>

namespace yq {
    /*! \brief Vector of 4 dimension(s)
    
        This is a 4 dimensional cartesian vector of the given type.
        \tparam[T]  Data type to be used, recommended to be arithmetic in nature
    */
    template <typename T>
    struct Vec4 {
        //! Component data type argument to this structure (ie, template parameter T)
        using component_type = T;

        /*! \brief Creates a unit-vector in the $(bit)s dimension.
        */
        static consteval Vec4 unit_x();
        /*! \brief Creates a unit-vector in the $(bit)s dimension.
        */
        static consteval Vec4 unit_y();
        /*! \brief Creates a unit-vector in the $(bit)s dimension.
        */
        static consteval Vec4 unit_z();
        /*! \brief Creates a unit-vector in the $(bit)s dimension.
        */
        static consteval Vec4 unit_w();

        //! Component for the $(bit)s dimension.
        T       x;
        //! Component for the $(bit)s dimension.
        T       y;
        //! Component for the $(bit)s dimension.
        T       z;
        //! Component for the $(bit)s dimension.
        T       w;;
        
        //! Equality operator (using default)
        constexpr bool operator==(const Vec4&) const noexcept = default;
    };
    
//  --------------------------------------------------------
//  COMPOSITION

    /*! \brief Creates a $(N)d dimensioal vector
    
        Helper function to create a $(N) dimensional cartesian vector where the component type is deduced from
        the first argument.
    */
    template <typename T>
    constexpr Vec4<T> vec(T x, std::type_identity_t<T> y, std::type_identity_t<T> z, std::type_identity_t<T> w)
    {
        return {x,y,z,w};
    }
    
    template <typename T>
    consteval Vec4<T> Vec4<T>::unit_x()
    {
        return {one_v<T>,zero_v<T>,zero_v<T>,zero_v<T>};
    }

    template <typename T>
    consteval Vec4<T> Vec4<T>::unit_y()
    {
        return {zero_v<T>,one_v<T>,zero_v<T>,zero_v<T>};
    }

    template <typename T>
    consteval Vec4<T> Vec4<T>::unit_z()
    {
        return {zero_v<T>,zero_v<T>,one_v<T>,zero_v<T>};
    }

    template <typename T>
    consteval Vec4<T> Vec4<T>::unit_w()
    {
        return {zero_v<T>,zero_v<T>,zero_v<T>,one_v<T>};
    }

    YQ_NAN_1(Vec4, Vec4<T>{nan_v<T>, nan_v<T>, nan_v<T>, nan_v<T>})
    YQ_ZERO_1(Vec4, Vec4<T>{zero_v<T>, zero_v<T>, zero_v<T>, zero_v<T>})
    
//  --------------------------------------------------------
//  BASIC FUNCTIONS

    YQ_IS_NAN_1(Vec4, is_nan(v.x) || is_nan(v.y) || is_nan(v.z) || is_nan(v.w))
    YQ_IS_FINITE_1(Vec4, is_finite(v.x) && is_finite(v.y) && is_finite(v.z) && is_finite(v.w))
        
    /*! \brief Square of the vector's length
    
        This returns the SQUARE of the given vector's length.
    */
    template <typename T>
    constexpr square_t<T> length2(const Vec4<T>& a)
    {
        return a.x*a.x + a.y*a.y + a.z*a.z + a.w*a.w;
    }    
    
    /*! \brief Length of the vector
        
        This returns the length of the given vector.
    */
    template <typename T>
    requires trait::has_sqrt_v<T>
    auto    length(const Vec4<T>& a)
    {
        return sqrt(length2(a));
    }

//  --------------------------------------------------------
//  POSITIVE

    template <typename T>
    constexpr Vec4<T> operator+(const Vec4<T>& a)
    {
        return a;
    }

//  --------------------------------------------------------
//  NEGATIVE

    template <typename T>
    constexpr Vec4<T> operator-(const Vec4<T>&a)
    {
        return {-a.x,-a.y,-a.z,-a.w};
    }

//  --------------------------------------------------------
//  NORMALIZATION

    template <typename T>
    requires trait::has_sqrt_v<T>
    Vec4<quotient_t<T,T>> operator~(const Vec4<T>& a)
    {
        auto l = one_v<T>/length(a);
        return {a.x/l, a.y/l, a.z/l, a.w/l};
    }

//  --------------------------------------------------------
//  ADDITION

    template <typename T>
    constexpr Vec4<T> operator+(const Vec4<T>& a, const Vec4<T>& b)
    {
        return {a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};
    }
    
    template <typename T>
    Vec4<T>& operator+=(Vec4<T>& a, const Vec4<T>& b)
    {
        a.x += b.x;
        a.y += b.y;
        a.z += b.z;
        a.w += b.w;
        return a;
    }

//  --------------------------------------------------------
//  SUBTRACTION

    template <typename T>
    constexpr Vec4<T> operator-(const Vec4<T>& a, const Vec4<T>& b)
    {
        return {a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};
    }
    
    template <typename T>
    Vec4<T>& operator-=(Vec4<T>& a, const Vec4<T>& b)
    {
        a.x -= b.x;
        a.y -= b.y;
        a.z -= b.z;
        a.w -= b.w;
        return a;
    }


//  --------------------------------------------------------
//  MULTIPLICATION

    template <typename T, typename U>
    requires (std::is_arithmetic_v<T>)
    constexpr Vec4<product_t<T,U>> operator*(T a, const Vec4<U>&b)
    {
        return {a*b.x, a*b.y, a*b.z, a*b.w};
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U>)
    constexpr Vec4<product_t<T,U>> operator*(const Vec4<T>& a, U b)
    {
        return {a.x*b, a.y*b, a.z*b, a.w*b};
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_mul_v<T,U>)
    Vec4<T>& operator*=(Vec4<T>& a, T b)
    {
        a.w *= b;
        return a;
    }

//  --------------------------------------------------------
//  DIVISION

    template <typename T, typename U>
    requires (std::is_arithmetic_v<T>)
    constexpr  Vec4<quotient_t<T,U>> operator/(T a, const  Vec4<U>&b)
    {
        return (a*b) / length2(b);
    }

    template <typename T, typename U>
    requires (std::is_arithmetic_v<U>)
    constexpr  Vec4<quotient_t<T,U>> operator/(const  Vec4<T>& a, U b)
    {
        return {a.x / b, a.y / b, a.z / b, a.w / b};
    }

    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_div_v<T,U>)
    Vec4<T>& operator/=(Vec4<T>& a, U b)
    {
        a.x /= b;
        a.y /= b;
        a.z /= b;
        a.w /= b;
        return a;
    }


//  --------------------------------------------------------
//  DOT PRODUCT

    template <typename T, typename U>
    constexpr product_t<T,U> operator DOT (const Vec4<T>& a, const Vec4<U>&b)
    {
        return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
    }

//  --------------------------------------------------------
//  INNER PRODUCT

    template <typename T, typename U>
    constexpr product_t<T,U> operator INNER (const Vec4<T>& a, const Vec4<U>&b)
    {
        return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
    }
}

YQ_TYPE_DECLARE(yq::Vec4D)
YQ_TYPE_DECLARE(yq::Vec4F)
YQ_TYPE_DECLARE(yq::Vec4I)
YQ_TYPE_DECLARE(yq::Vec4U)

