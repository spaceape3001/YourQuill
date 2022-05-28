////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

//  WARNING!  WARNING!
//
//  THIS FILE IS AUTO-GENERATED, CHANGES WILL BE CLOBBERED!

#define YQ__API__MATH__TENSOR_1_4__HPP 1
#include <math/preamble.hpp>
#include <math/vec/Vec1.hpp>
#include <math/vec/Vec4.hpp>

namespace yq {
    /*! \brief 1x4 second order tensor (ie a matrix)
    
        \tparam[T]  Data type to be used, recommended to be arithmetic in nature
    */
    template <typename T>
    struct Ten14 {
        using component_type = T;
        T xx, xy, xz, xw;

        constexpr bool operator==(const Ten14&) const noexcept = default;
    };
        
//  --------------------------------------------------------
//  COMPOSITION

    template <typename T>
    constexpr Ten14<T>  columns(const Vec1<T>&x, const Vec1<T>&y, const Vec1<T>&z, const Vec1<T>&w)
    {
        return {
            x.x, y.x, z.x, w.x
        };
    }

    template <typename T>
    constexpr Ten14<T>  rows(const Vec4<T>&x)
    {
        return {
            x.x, x.y, x.z, x.w
        };
    }
    
    YQ_IDENTITY_1(Ten14, {
        one_v<T>, zero_v<T>, zero_v<T>, zero_v<T>
    })

    YQ_NAN_1(Ten14, {
        nan_v<T>, nan_v<T>, nan_v<T>, nan_v<T> 
    })
    
    YQ_ZERO_1(Ten14, {
        zero_v<T>, zero_v<T>, zero_v<T>, zero_v<T> 
     })
    
//  --------------------------------------------------------
//  BASIC FUNCTIONS


    YQ_IS_FINITE_1(Ten14, 
        is_finite(v.xx) && is_finite(v.xy) && is_finite(v.xz) && is_finite(v.xw)
    )
    
    YQ_IS_NAN_1(Ten14,  
        is_nan(v.xx) || is_nan(v.xy) || is_nan(v.xz) || is_nan(v.xw)
    )

//  --------------------------------------------------------
//  GETTERS

    template <typename T>
    constexpr Vec1<T>  x_column(const Ten14<T>&v) 
    {
        return {v.xx};
    }

    template <typename T>
    constexpr Vec1<T>  y_column(const Ten14<T>&v) 
    {
        return {v.xy};
    }

    template <typename T>
    constexpr Vec1<T>  z_column(const Ten14<T>&v) 
    {
        return {v.xz};
    }

    template <typename T>
    constexpr Vec1<T>  w_column(const Ten14<T>&v) 
    {
        return {v.xw};
    }

    template <typename T>
    constexpr Vec4<T>  x_row(const Ten14<T>&v)
    {
        return {v.xx, v.xy, v.xz, v.xw};
    }


//  --------------------------------------------------------
//  SETTERS

    template <typename T>
    Ten14<T>& set_x_column(Ten14<T>&ten, const Vec4<T>& v)
    {
        ten.xx = v.x;
        return ten;
    }

    template <typename T>
    Ten14<T>& set_y_column(Ten14<T>&ten, const Vec4<T>& v)
    {
        ten.xy = v.x;
        return ten;
    }

    template <typename T>
    Ten14<T>& set_z_column(Ten14<T>&ten, const Vec4<T>& v)
    {
        ten.xz = v.x;
        return ten;
    }

    template <typename T>
    Ten14<T>& set_w_column(Ten14<T>&ten, const Vec4<T>& v)
    {
        ten.xw = v.x;
        return ten;
    }

    template <typename T>
    Ten14<T>& set_x_row(Ten14<T>&ten, const Vec1<T>& v)
    {
        ten.xx = v.x;
        ten.xy = v.y;
        ten.xz = v.z;
        ten.xw = v.w;
        return ten;
    }

//  --------------------------------------------------------
//  POSITIVE

    template <typename T>
    constexpr Ten14<T>  operator+(const Ten14<T>& a) 
    { 
        return a; 
    }


//  --------------------------------------------------------
//  NEGATIVE

    template <typename T>
    constexpr Ten14<T>  operator-(const Ten14<T>& a) 
    {
        return {
            -a.xx, -a.xy, -a.xz, -a.xw
        };
    }


//  --------------------------------------------------------
//  ADDITION

    template <typename T>
    constexpr Ten14<T>   operator+ (const Ten14<T> &a, const Ten14<T> &b) 
    {
        return {
            a.xx+b.xx, a.xy+b.xy, a.xz+b.xz, a.xw+b.xw
        };
    }

    template <typename T>
    Ten14<T>&   operator+=(Ten14<T> &a, const Ten14<T> &b) 
    {
        a.xx+=b.xx;  a.xy+=b.xy;  a.xz+=b.xz;  a.xw+=b.xw;
        return a;
    }


//  --------------------------------------------------------
//  SUBTRACTION

    template <typename T>
    constexpr Ten14<T>   operator- (const Ten14<T> &a, const Ten14<T> &b) 
    {
        return {
            a.xx-b.xx, a.xy-b.xy, a.xz-b.xz, a.xw-b.xw
        };
    }
    

    template <typename T>
    Ten14<T>&   operator-=(Ten14<T> &a, const Ten14<T> &b) 
    {
        a.xx-=b.xx;  a.xy-=b.xy;  a.xz-=b.xz;  a.xw-=b.xw;
        return a;
    }
    
//  --------------------------------------------------------
//  MULTIPLICATION

    template <typename T, typename U>
    requires std::is_arithmetic_v<T>
    constexpr Ten14<product_t<T,U>>  operator*(T a, const Ten14<T>& b)
    {
        return {
            a*b.xx, a*b.xy, a*b.xz, a*b.xw
        };
    }
    
    
    template <typename T, typename U>
    requires std::is_arithmetic_v<U>
    constexpr Ten14<product_t<T,U>>  operator*(const Ten14<T>& a, U b)
    {
        return {
            a.xx*b, a.xy*b, a.xz*b, a.xw*b
        };
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_mul_v<T,U>)
    Ten14<product_t<T,U>>  operator*=(const Ten14<T>& a, U b)
    {
        a.xx*=b; a.xy*=b; a.xz*=b; a.xw*=b;        
        return a;
    }

        
    template <typename T, typename U>
    constexpr Vec1<product_t<T,U>> operator*(const Ten14<T>&a, const Vec4<U>&b)
    {
        return {
            a.xx*b.x + a.xy*b.y + a.xz*b.z + a.xw*b.w
        };
    }

    template <typename T, typename U>
    constexpr Vec4<product_t<T,U>> operator*(const Vec1<T>&a, const Ten14<U>&b)
    {
        return {
            a.x*b.xx,
            a.x*b.xy,
            a.x*b.xz,
            a.x*b.xw
        };
    }

//  --------------------------------------------------------
//  DIVISION


    template <typename T, typename U>
    requires std::is_arithmetic_v<U>
    constexpr Ten14<quotient_t<T,U>>  operator/(const Ten14<T>& a, U b)
    {
        return {
            a.xx/b, a.xy/b, a.xz/b, a.xw/b
        };
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_div_v<T,U>)
    Ten14<quotient_t<T,U>>  operator/=(const Ten14<T>& a, U b)
    {
        a.xx/=b; a.xy/=b; a.xz/=b; a.xw/=b;        
        return a;
    }

//  --------------------------------------------------------
//  OTIMES PRODUCT

    template <typename T, typename U>
    constexpr Ten14<product_t<T,U>> operator OTIMES(const Vec1<T>&a, const Vec4<U>&b)
    {
        return {
            a.x+b.x, a.x+b.y, a.x+b.z, a.x+b.w
        };
    }

//  --------------------------------------------------------
//  ADVANCED FUNCTIONS

}

YQ_TYPE_DECLARE(yq::Ten14D)
YQ_TYPE_DECLARE(yq::Ten14F)
YQ_TYPE_DECLARE(yq::Ten14I)
YQ_TYPE_DECLARE(yq::Ten14U)

