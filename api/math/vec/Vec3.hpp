////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

//  WARNING!  WARNING!
//
//  THIS FILE IS AUTO-GENERATED, CHANGES WILL BE CLOBBERED!

#define YQ__API__MATH__VECTOR_3__HPP 1
#include <math/preamble.hpp>

namespace yq {
    /*! \brief Vector of 3 dimension(s)
    
        This is a 3 dimensional cartesian vector of the given type.
        \tparam[T]  Data type to be used, recommended to be arithmetic in nature
    */
    template <typename T>
    struct Vec3 {
        //! Component data type argument to this structure (ie, template parameter T)
        using component_type = T;

        /*! \brief Creates a unit-vector in the $(bit)s dimension.
        */
        static consteval Vec3 unit_x();
        /*! \brief Creates a unit-vector in the $(bit)s dimension.
        */
        static consteval Vec3 unit_y();
        /*! \brief Creates a unit-vector in the $(bit)s dimension.
        */
        static consteval Vec3 unit_z();

        //! Component for the $(bit)s dimension.
        T       x;
        //! Component for the $(bit)s dimension.
        T       y;
        //! Component for the $(bit)s dimension.
        T       z;;
        
        //! Equality operator (using default)
        constexpr bool operator==(const Vec3&) const noexcept = default;
    };
    
//  --------------------------------------------------------
//  COMPOSITION

    /*! \brief Creates a $(N)d dimensioal vector
    
        Helper function to create a $(N) dimensional cartesian vector where the component type is deduced from
        the first argument.
    */
    template <typename T>
    constexpr Vec3<T> vec(T x, std::type_identity_t<T> y, std::type_identity_t<T> z)
    {
        return {x,y,z};
    }
    
    template <typename T>
    consteval Vec3<T> Vec3<T>::unit_x()
    {
        return {one_v<T>,zero_v<T>,zero_v<T>};
    }

    template <typename T>
    consteval Vec3<T> Vec3<T>::unit_y()
    {
        return {zero_v<T>,one_v<T>,zero_v<T>};
    }

    template <typename T>
    consteval Vec3<T> Vec3<T>::unit_z()
    {
        return {zero_v<T>,zero_v<T>,one_v<T>};
    }

    YQ_NAN_1(Vec3, Vec3<T>{nan_v<T>, nan_v<T>, nan_v<T>})
    YQ_ZERO_1(Vec3, Vec3<T>{zero_v<T>, zero_v<T>, zero_v<T>})
    
//  --------------------------------------------------------
//  BASIC FUNCTIONS

    YQ_IS_NAN_1(Vec3, is_nan(v.x) || is_nan(v.y) || is_nan(v.z))
    YQ_IS_FINITE_1(Vec3, is_finite(v.x) && is_finite(v.y) && is_finite(v.z))
        
    /*! \brief Square of the vector's length
    
        This returns the SQUARE of the given vector's length.
    */
    template <typename T>
    constexpr square_t<T> length2(const Vec3<T>& a)
    {
        return a.x*a.x + a.y*a.y + a.z*a.z;
    }    
    
    /*! \brief Length of the vector
        
        This returns the length of the given vector.
    */
    template <typename T>
    requires trait::has_sqrt_v<T>
    auto    length(const Vec3<T>& a)
    {
        return sqrt(length2(a));
    }

//  --------------------------------------------------------
//  POSITIVE

    template <typename T>
    constexpr Vec3<T> operator+(const Vec3<T>& a)
    {
        return a;
    }

//  --------------------------------------------------------
//  NEGATIVE

    template <typename T>
    constexpr Vec3<T> operator-(const Vec3<T>&a)
    {
        return {-a.x,-a.y,-a.z};
    }

//  --------------------------------------------------------
//  NORMALIZATION

    template <typename T>
    requires trait::has_sqrt_v<T>
    Vec3<quotient_t<T,T>> operator~(const Vec3<T>& a)
    {
        auto l = one_v<T>/length(a);
        return {a.x/l, a.y/l, a.z/l};
    }

//  --------------------------------------------------------
//  ADDITION

    template <typename T>
    constexpr Vec3<T> operator+(const Vec3<T>& a, const Vec3<T>& b)
    {
        return {a.x+b.x, a.y+b.y, a.z+b.z};
    }
    
    template <typename T>
    Vec3<T>& operator+=(Vec3<T>& a, const Vec3<T>& b)
    {
        a.x += b.x;
        a.y += b.y;
        a.z += b.z;
        return a;
    }

//  --------------------------------------------------------
//  SUBTRACTION

    template <typename T>
    constexpr Vec3<T> operator-(const Vec3<T>& a, const Vec3<T>& b)
    {
        return {a.x-b.x, a.y-b.y, a.z-b.z};
    }
    
    template <typename T>
    Vec3<T>& operator-=(Vec3<T>& a, const Vec3<T>& b)
    {
        a.x -= b.x;
        a.y -= b.y;
        a.z -= b.z;
        return a;
    }


//  --------------------------------------------------------
//  MULTIPLICATION

    template <typename T, typename U>
    requires (std::is_arithmetic_v<T>)
    constexpr Vec3<product_t<T,U>> operator*(T a, const Vec3<U>&b)
    {
        return {a*b.x, a*b.y, a*b.z};
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U>)
    constexpr Vec3<product_t<T,U>> operator*(const Vec3<T>& a, U b)
    {
        return {a.x*b, a.y*b, a.z*b};
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_mul_v<T,U>)
    Vec3<T>& operator*=(Vec3<T>& a, T b)
    {
        a.z *= b;
        return a;
    }

    template <typename T, typename U>
    Vec3<product_t<T,U>>    mul_elem(const Vec3<T>&a, const Vec3<T>&b)
    {
        return {a.x*b.x, a.y*b.y, a.z*b.z};
    }
    
//  --------------------------------------------------------
//  DIVISION

    template <typename T, typename U>
    requires (std::is_arithmetic_v<T>)
    constexpr  Vec3<quotient_t<T,U>> operator/(T a, const  Vec3<U>&b)
    {
        return (a*b) / length2(b);
    }

    template <typename T, typename U>
    requires (std::is_arithmetic_v<U>)
    constexpr  Vec3<quotient_t<T,U>> operator/(const  Vec3<T>& a, U b)
    {
        return {a.x / b, a.y / b, a.z / b};
    }

    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_div_v<T,U>)
    Vec3<T>& operator/=(Vec3<T>& a, U b)
    {
        a.x /= b;
        a.y /= b;
        a.z /= b;
        return a;
    }

    template <typename T, typename U>
    Vec3<quotient_t<T,U>>    div_elem(const Vec3<T>&a, const Vec3<T>&b)
    {
        return {a.x/b.x, a.y/b.y, a.z/b.z};
    }

//  --------------------------------------------------------
//  DOT PRODUCT

    template <typename T, typename U>
    constexpr product_t<T,U> operator DOT (const Vec3<T>& a, const Vec3<U>&b)
    {
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }

//  --------------------------------------------------------
//  INNER PRODUCT

    template <typename T, typename U>
    constexpr product_t<T,U> operator INNER (const Vec3<T>& a, const Vec3<U>&b)
    {
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }

    //  --------------------------------------------------------
    //  ADVANCED FUNCTIONS

    template <typename T>
    Vec3<T>   abs_elem(const Vec3<T>&a, const Vec3<T>&b)
    {
        return {abs(a.x, b.x) && abs(a.y, b.y) && abs(a.z, b.z)};
    }

    //! TRUE if every component of a is less than b
    template <typename T>
    constexpr bool        all_less(const Vec3<T>& a, const Vec3<T>&b)
    {
        return (a.x<b.x) && (a.y<b.y) && (a.z<b.z);
    }

    //! TRUE if every component of a is less than (or equal to) b
    template <typename T>
    constexpr bool        all_less_equal(const Vec3<T>& a, const Vec3<T>&b)
    {
        return (a.x<=b.x) && (a.y<=b.y) && (a.z<=b.z);
    }

    //! TRUE if every component of a is greater than b
    template <typename T>
    constexpr bool        all_greater(const Vec3<T>& a, const Vec3<T>&b)
    {
        return (a.x>b.x) && (a.y>b.y) && (a.z>b.z);
    }

    //! TRUE if every component of a is greater or equal to b
    template <typename T>
    constexpr bool        all_greater_equal(const Vec3<T>& a, const Vec3<T>&b)
    {
        return (a.x>=b.x) && (a.y>=b.y) && (a.z>=b.z);
    }
    
    template <typename T>
    constexpr Vec3<T>   max_elem(const Vec3<T>&a, const Vec3<T>&b)
    {
        return {max(a.x, b.x) && max(a.y, b.y) && max(a.z, b.z)};
    }
    
    template <typename T>
    constexpr Vec3<T>   min_elem(const Vec3<T>&a, const Vec3<T>&b)
    {
        return {min(a.x, b.x) && min(a.y, b.y) && min(a.z, b.z)};
    }
}

YQ_TYPE_DECLARE(yq::Vec3D)
YQ_TYPE_DECLARE(yq::Vec3F)
YQ_TYPE_DECLARE(yq::Vec3I)
YQ_TYPE_DECLARE(yq::Vec3U)

