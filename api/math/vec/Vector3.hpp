////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#define YQ__API__MATH__VECTOR_3__HPP 1
#include <math/preamble.hpp>
#include <math/trig.hpp>

namespace yq {
    /*! \brief Vector of 3 dimension(s)
    
        This is a 3 dimensional cartesian vector of the given type.
        \tparam[T]  Data type to be used, recommended to be arithmetic in nature
    */
    template <typename T>
    struct Vector3 {
        //! Component data type argument to this structure (ie, template parameter T)
        using component_type = T;

        /*! \brief Creates a unit-vector in the $(bit)s dimension.
        */
        static consteval Vector3 unit_x();
        /*! \brief Creates a unit-vector in the $(bit)s dimension.
        */
        static consteval Vector3 unit_y();
        /*! \brief Creates a unit-vector in the $(bit)s dimension.
        */
        static consteval Vector3 unit_z();

        //! Component for the $(bit)s dimension.
        T       x;
        //! Component for the $(bit)s dimension.
        T       y;
        //! Component for the $(bit)s dimension.
        T       z;;
        
        //! Equality operator (using default)
        constexpr bool operator==(const Vector3&) const noexcept = default;

        constexpr operator glm::vec<3, T, glm::defaultp>() const noexcept
        {
            return { x, y, z };
        }
    };

    YQ_IEEE754_1(Vector3)
    
//  --------------------------------------------------------
//  COMPOSITION

    /*! \brief Creates a $(N)d dimensioal vector
    
        Helper function to create a $(N) dimensional cartesian vector where the component type is deduced from
        the first argument.
    */
    template <typename T>
    constexpr Vector3<T> vector(T x, std::type_identity_t<T> y, std::type_identity_t<T> z)
    {
        return {x,y,z};
    }
    
    template <typename T, glm::qualifier Q>
    constexpr Vector3<T> vector(const glm::vec<3,T,Q>& v)
    {
        return { v.x, v.y, v.z };
    }

    //! Creates a two dimension unit vector
    //!
    //! \param a    Counter-clockwise angle from +x
    template <typename T>
    requires std::is_floating_point_v<T>
    Vector3<T>  ccw(MKS<T,dim::Angle> a, MKS<T,dim::Angle> b)
    {
        auto cb = cos(b);
        return { cb*cos(a), cb*sin(a), sin(b) };
    }

    template <typename T>
    consteval Vector3<T> Vector3<T>::unit_x()
    {
        return {one_v<T>,zero_v<T>,zero_v<T>};
    }

    template <typename T>
    consteval Vector3<T> Vector3<T>::unit_y()
    {
        return {zero_v<T>,one_v<T>,zero_v<T>};
    }

    template <typename T>
    consteval Vector3<T> Vector3<T>::unit_z()
    {
        return {zero_v<T>,zero_v<T>,one_v<T>};
    }

    YQ_NAN_1(Vector3, Vector3<T>{nan_v<T>, nan_v<T>, nan_v<T>})
    YQ_ZERO_1(Vector3, Vector3<T>{zero_v<T>, zero_v<T>, zero_v<T>})
    
//  --------------------------------------------------------
//  BASIC FUNCTIONS

    YQ_IS_NAN_1(Vector3, is_nan(v.x) || is_nan(v.y) || is_nan(v.z))
    YQ_IS_FINITE_1(Vector3, is_finite(v.x) && is_finite(v.y) && is_finite(v.z))
        
    /*! \brief Square of the vector's length
    
        This returns the SQUARE of the given vector's length.
    */
    template <typename T>
    constexpr square_t<T> length2(const Vector3<T>& a)
    {
        return a.x*a.x + a.y*a.y + a.z*a.z;
    }    
    
    template <typename T>
    constexpr square_t<T> operator^(const Vector3<T>& a,two_t)
    {
        return a.x*a.x + a.y*a.y + a.z*a.z;
    }    

    /*! \brief Length of the vector
        
        This returns the length of the given vector.
    */
    template <typename T>
    requires trait::has_sqrt_v<T>
    auto    length(const Vector3<T>& a)
    {
        return sqrt(length2(a));
    }

//  --------------------------------------------------------
//  POSITIVE

    template <typename T>
    constexpr Vector3<T> operator+(const Vector3<T>& a)
    {
        return a;
    }

//  --------------------------------------------------------
//  NEGATIVE

    template <typename T>
    constexpr Vector3<T> operator-(const Vector3<T>&a)
    {
        return {-a.x,-a.y,-a.z};
    }

//  --------------------------------------------------------
//  NORMALIZATION

    template <typename T>
    requires trait::has_sqrt_v<T>
    Vector3<quotient_t<T,T>> operator~(const Vector3<T>& a)
    {
        auto l = one_v<T>/length(a);
        return {a.x/l, a.y/l, a.z/l};
    }

//  --------------------------------------------------------
//  ADDITION

    template <typename T>
    constexpr Vector3<T> operator+(const Vector3<T>& a, const Vector3<T>& b)
    {
        return {a.x+b.x, a.y+b.y, a.z+b.z};
    }
    
    template <typename T>
    Vector3<T>& operator+=(Vector3<T>& a, const Vector3<T>& b)
    {
        a.x += b.x;
        a.y += b.y;
        a.z += b.z;
        return a;
    }

//  --------------------------------------------------------
//  SUBTRACTION

    template <typename T>
    constexpr Vector3<T> operator-(const Vector3<T>& a, const Vector3<T>& b)
    {
        return {a.x-b.x, a.y-b.y, a.z-b.z};
    }
    
    template <typename T>
    Vector3<T>& operator-=(Vector3<T>& a, const Vector3<T>& b)
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
    constexpr Vector3<product_t<T,U>> operator*(T a, const Vector3<U>&b)
    {
        return {a*b.x, a*b.y, a*b.z};
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U>)
    constexpr Vector3<product_t<T,U>> operator*(const Vector3<T>& a, U b)
    {
        return {a.x*b, a.y*b, a.z*b};
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_mul_v<T,U>)
    Vector3<T>& operator*=(Vector3<T>& a, T b)
    {
        a.z *= b;
        return a;
    }

    template <typename T, typename U>
    Vector3<product_t<T,U>>    mul_elem(const Vector3<T>&a, const Vector3<T>&b)
    {
        return {a.x*b.x, a.y*b.y, a.z*b.z};
    }
    
//  --------------------------------------------------------
//  DIVISION

    template <typename T, typename U>
    requires (std::is_arithmetic_v<T>)
    constexpr  Vector3<quotient_t<T,U>> operator/(T a, const  Vector3<U>&b)
    {
        return (a*b) / length2(b);
    }

    template <typename T, typename U>
    requires (std::is_arithmetic_v<U>)
    constexpr  Vector3<quotient_t<T,U>> operator/(const  Vector3<T>& a, U b)
    {
        return {a.x / b, a.y / b, a.z / b};
    }

    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_div_v<T,U>)
    Vector3<T>& operator/=(Vector3<T>& a, U b)
    {
        a.x /= b;
        a.y /= b;
        a.z /= b;
        return a;
    }

    template <typename T, typename U>
    Vector3<quotient_t<T,U>>    div_elem(const Vector3<T>&a, const Vector3<T>&b)
    {
        return {a.x/b.x, a.y/b.y, a.z/b.z};
    }

//  --------------------------------------------------------
//  DOT PRODUCT

    template <typename T, typename U>
    constexpr product_t<T,U> operator DOT (const Vector3<T>& a, const Vector3<U>&b)
    {
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }

//  --------------------------------------------------------
//  INNER PRODUCT

    template <typename T, typename U>
    constexpr product_t<T,U> operator INNER (const Vector3<T>& a, const Vector3<U>&b)
    {
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }

    //  --------------------------------------------------------
    //  ADVANCED FUNCTIONS

    template <typename T>
    Vector3<T>   abs_elem(const Vector3<T>&a, const Vector3<T>&b)
    {
        return {abs(a.x, b.x) && abs(a.y, b.y) && abs(a.z, b.z)};
    }

    //! TRUE if every component of a is less than b
    template <typename T>
    constexpr bool        all_less(const Vector3<T>& a, const Vector3<T>&b)
    {
        return (a.x<b.x) && (a.y<b.y) && (a.z<b.z);
    }

    //! TRUE if every component of a is less than (or equal to) b
    template <typename T>
    constexpr bool        all_less_equal(const Vector3<T>& a, const Vector3<T>&b)
    {
        return (a.x<=b.x) && (a.y<=b.y) && (a.z<=b.z);
    }

    //! TRUE if every component of a is greater than b
    template <typename T>
    constexpr bool        all_greater(const Vector3<T>& a, const Vector3<T>&b)
    {
        return (a.x>b.x) && (a.y>b.y) && (a.z>b.z);
    }

    //! TRUE if every component of a is greater or equal to b
    template <typename T>
    constexpr bool        all_greater_equal(const Vector3<T>& a, const Vector3<T>&b)
    {
        return (a.x>=b.x) && (a.y>=b.y) && (a.z>=b.z);
    }
    
    template <typename T>
    requires (std::is_floating_point_v<T> && trait::has_sqrt_v<T>)
    MKS<T,dim::Angle>       angle(const Vector3<T>&a, const Vector3<T>& b)
    {
        return acos( std::clamp<T>( (a*b)/(length(a)*length(b)), -one_v<T>, one_v<T>));
    }
    
    template <typename T, typename DIM1, typename DIM2>
    requires (std::is_floating_point_v<T> && trait::has_sqrt_v<T>)
    MKS<T,dim::Angle>       angle(const Vector3<MKS<T,DIM1>>&a, const Vector3<MKS<T,DIM2>>& b)
    {
        using one_t = MKS<T,dim::None>;
        return acos( std::clamp<one_t>( (a*b)/(length(a)*length(b)), -one_v<T>, one_v<T>));
    }
    
    /*! \brief Counter clockwise (euler) angle
    
        Computes the euler angle of the vector, ie, counter-clockwise from the +X axis.
    */
    template <typename T>
    requires std::is_floating_point_v<T>
    MKS<T,dim::Angle>   ccw(const Vector3<T>& a)
    {
        return atan(a.y, a.x);
    }

    /*! \brief Counter clockwise (euler) angle
    
        Computes the euler angle of the vector, ie, counter-clockwise from the +X axis.
    */
    template <typename T, typename DIM>
    requires std::is_floating_point_v<T>
    MKS<T,dim::Angle>   ccw(const Vector3<MKS<T,DIM>>& a)
    {
        return atan(a.y, a.x);
    }

    template <typename T>
    cube_t<T>       component_product(const Vector3<T>& a)
    {
        return a.x*a.y*a.z;
    }

    /*! \brief Clockwise angle
    
        Computes the angle of the vector from the +Y axis.
    */
    template <typename T>
    requires std::is_floating_point_v<T>
    MKS<T,dim::Angle>   cw(const Vector3<T>& a)
    {
        return atan(a.y, a.x);
    }

    /*! \brief Clockwise angle
    
        Computes the angle of the vector from the +Y axis.
    */
    template <typename T, typename DIM>
    requires std::is_floating_point_v<T>
    MKS<T,dim::Angle>   cw(const Vector3<MKS<T,DIM>>& a)
    {
        return atan(a.y, a.x);
    }
    
    template <typename T>
    constexpr Vector3<T>   max_elem(const Vector3<T>&a, const Vector3<T>&b)
    {
        return {max(a.x, b.x) && max(a.y, b.y) && max(a.z, b.z)};
    }
    
    template <typename T>
    constexpr Vector3<T>   min_elem(const Vector3<T>&a, const Vector3<T>&b)
    {
        return {min(a.x, b.x) && min(a.y, b.y) && min(a.z, b.z)};
    }
}

YQ_TYPE_DECLARE(yq::Vector3D)
YQ_TYPE_DECLARE(yq::Vector3F)
YQ_TYPE_DECLARE(yq::Vector3I)
YQ_TYPE_DECLARE(yq::Vector3U)

