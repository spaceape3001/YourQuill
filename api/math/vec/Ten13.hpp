////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

//  WARNING!  WARNING!
//
//  THIS FILE IS AUTO-GENERATED, CHANGES WILL BE CLOBBERED!

#define YQ__API__MATH__TENSOR_1_3__HPP 1
#include <math/preamble.hpp>
#include <math/vec/Vec1.hpp>
#include <math/vec/Vec3.hpp>

namespace yq {
    /*! \brief 1x3 second order tensor (ie a matrix)
    
        \tparam[T]  Data type to be used, recommended to be arithmetic in nature
    */
    template <typename T>
    struct Ten13 {
        using component_type = T;
        T xx, xy, xz;

        constexpr bool operator==(const Ten13&) const noexcept = default;
    };
        
//  --------------------------------------------------------
//  COMPOSITION

    template <typename T>
    constexpr Ten13<T>  columns(const Vec1<T>&x, const Vec1<T>&y, const Vec1<T>&z)
    {
        return {
            x.x, y.x, z.x
        };
    }

    template <typename T>
    constexpr Ten13<T>  rows(const Vec3<T>&x)
    {
        return {
            x.x, x.y, x.z
        };
    }
    
    YQ_IDENTITY_1(Ten13, {
        one_v<T>, zero_v<T>, zero_v<T>
    })

    YQ_NAN_1(Ten13, {
        nan_v<T>, nan_v<T>, nan_v<T> 
    })
    
    YQ_ZERO_1(Ten13, {
        zero_v<T>, zero_v<T>, zero_v<T> 
     })
    
//  --------------------------------------------------------
//  BASIC FUNCTIONS


    YQ_IS_FINITE_1(Ten13, 
        is_finite(v.xx) && is_finite(v.xy) && is_finite(v.xz)
    )
    
    YQ_IS_NAN_1(Ten13,  
        is_nan(v.xx) || is_nan(v.xy) || is_nan(v.xz)
    )

//  --------------------------------------------------------
//  GETTERS

    template <typename T>
    constexpr Vec1<T>  x_column(const Ten13<T>&v) 
    {
        return {v.xx};
    }

    template <typename T>
    constexpr Vec1<T>  y_column(const Ten13<T>&v) 
    {
        return {v.xy};
    }

    template <typename T>
    constexpr Vec1<T>  z_column(const Ten13<T>&v) 
    {
        return {v.xz};
    }

    template <typename T>
    constexpr Vec3<T>  x_row(const Ten13<T>&v)
    {
        return {v.xx, v.xy, v.xz};
    }


//  --------------------------------------------------------
//  SETTERS

    template <typename T>
    Ten13<T>& set_x_column(Ten13<T>&ten, const Vec3<T>& v)
    {
        ten.xx = v.x;
        return ten;
    }

    template <typename T>
    Ten13<T>& set_y_column(Ten13<T>&ten, const Vec3<T>& v)
    {
        ten.xy = v.x;
        return ten;
    }

    template <typename T>
    Ten13<T>& set_z_column(Ten13<T>&ten, const Vec3<T>& v)
    {
        ten.xz = v.x;
        return ten;
    }

    template <typename T>
    Ten13<T>& set_x_row(Ten13<T>&ten, const Vec1<T>& v)
    {
        ten.xx = v.x;
        ten.xy = v.y;
        ten.xz = v.z;
        return ten;
    }

//  --------------------------------------------------------
//  POSITIVE

    template <typename T>
    constexpr Ten13<T>  operator+(const Ten13<T>& a) 
    { 
        return a; 
    }


//  --------------------------------------------------------
//  NEGATIVE

    template <typename T>
    constexpr Ten13<T>  operator-(const Ten13<T>& a) 
    {
        return {
            -a.xx, -a.xy, -a.xz
        };
    }


//  --------------------------------------------------------
//  ADDITION

    template <typename T>
    constexpr Ten13<T>   operator+ (const Ten13<T> &a, const Ten13<T> &b) 
    {
        return {
            a.xx+b.xx, a.xy+b.xy, a.xz+b.xz
        };
    }

    template <typename T>
    Ten13<T>&   operator+=(Ten13<T> &a, const Ten13<T> &b) 
    {
        a.xx+=b.xx;  a.xy+=b.xy;  a.xz+=b.xz;
        return a;
    }


//  --------------------------------------------------------
//  SUBTRACTION

    template <typename T>
    constexpr Ten13<T>   operator- (const Ten13<T> &a, const Ten13<T> &b) 
    {
        return {
            a.xx-b.xx, a.xy-b.xy, a.xz-b.xz
        };
    }
    

    template <typename T>
    Ten13<T>&   operator-=(Ten13<T> &a, const Ten13<T> &b) 
    {
        a.xx-=b.xx;  a.xy-=b.xy;  a.xz-=b.xz;
        return a;
    }
    
//  --------------------------------------------------------
//  MULTIPLICATION

    template <typename T, typename U>
    requires std::is_arithmetic_v<T>
    constexpr Ten13<product_t<T,U>>  operator*(T a, const Ten13<T>& b)
    {
        return {
            a*b.xx, a*b.xy, a*b.xz
        };
    }
    
    
    template <typename T, typename U>
    requires std::is_arithmetic_v<U>
    constexpr Ten13<product_t<T,U>>  operator*(const Ten13<T>& a, U b)
    {
        return {
            a.xx*b, a.xy*b, a.xz*b
        };
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_mul_v<T,U>)
    Ten13<product_t<T,U>>  operator*=(const Ten13<T>& a, U b)
    {
        a.xx*=b; a.xy*=b; a.xz*=b;        
        return a;
    }

        
    template <typename T, typename U>
    constexpr Vec1<product_t<T,U>> operator*(const Ten13<T>&a, const Vec3<U>&b)
    {
        return {
            a.xx*b.x + a.xy*b.y + a.xz*b.z
        };
    }

    template <typename T, typename U>
    constexpr Vec3<product_t<T,U>> operator*(const Vec1<T>&a, const Ten13<U>&b)
    {
        return {
            a.x*b.xx,
            a.x*b.xy,
            a.x*b.xz
        };
    }

//  --------------------------------------------------------
//  DIVISION


    template <typename T, typename U>
    requires std::is_arithmetic_v<U>
    constexpr Ten13<quotient_t<T,U>>  operator/(const Ten13<T>& a, U b)
    {
        return {
            a.xx/b, a.xy/b, a.xz/b
        };
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_div_v<T,U>)
    Ten13<quotient_t<T,U>>  operator/=(const Ten13<T>& a, U b)
    {
        a.xx/=b; a.xy/=b; a.xz/=b;        
        return a;
    }

//  --------------------------------------------------------
//  OTIMES PRODUCT

    template <typename T, typename U>
    constexpr Ten13<product_t<T,U>> operator OTIMES(const Vec1<T>&a, const Vec3<U>&b)
    {
        return {
            a.x+b.x, a.x+b.y, a.x+b.z
        };
    }

//  --------------------------------------------------------
//  ADVANCED FUNCTIONS

}

YQ_TYPE_DECLARE(yq::Ten13D)
YQ_TYPE_DECLARE(yq::Ten13F)
YQ_TYPE_DECLARE(yq::Ten13I)
YQ_TYPE_DECLARE(yq::Ten13U)

