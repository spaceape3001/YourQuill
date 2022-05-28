////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

//  WARNING!  WARNING!
//
//  THIS FILE IS AUTO-GENERATED, CHANGES WILL BE CLOBBERED!

#define YQ__API__MATH__TENSOR_3_2__HPP 1
#include <math/preamble.hpp>
#include <math/vec/Vec2.hpp>
#include <math/vec/Vec3.hpp>

namespace yq {
    /*! \brief 3x2 second order tensor (ie a matrix)
    
        \tparam[T]  Data type to be used, recommended to be arithmetic in nature
    */
    template <typename T>
    struct Ten32 {
        using component_type = T;
        T xx, xy;
        T yx, yy;
        T zx, zy;

        constexpr bool operator==(const Ten32&) const noexcept = default;
    };
        
//  --------------------------------------------------------
//  COMPOSITION

    template <typename T>
    constexpr Ten32<T>  columns(const Vec3<T>&x, const Vec3<T>&y)
    {
        return {
            x.x, y.x,
            x.y, y.y,
            x.z, y.z
        };
    }

    template <typename T>
    constexpr Ten32<T>  rows(const Vec2<T>&x, const Vec2<T>&y, const Vec2<T>&z)
    {
        return {
            x.x, x.y,
            y.x, y.y,
            z.x, z.y
        };
    }
    
    YQ_IDENTITY_1(Ten32, {
        one_v<T>, zero_v<T>,
        zero_v<T>, one_v<T>,
        zero_v<T>, zero_v<T>
    })

    YQ_NAN_1(Ten32, {
        nan_v<T>, nan_v<T>,
        nan_v<T>, nan_v<T>,
        nan_v<T>, nan_v<T> 
    })
    
    YQ_ZERO_1(Ten32, {
        zero_v<T>, zero_v<T>,
        zero_v<T>, zero_v<T>,
        zero_v<T>, zero_v<T> 
     })
    
//  --------------------------------------------------------
//  BASIC FUNCTIONS


    YQ_IS_FINITE_1(Ten32, 
        is_finite(v.xx) && is_finite(v.xy) &&
        is_finite(v.yx) && is_finite(v.yy) &&
        is_finite(v.zx) && is_finite(v.zy)
    )
    
    YQ_IS_NAN_1(Ten32,  
        is_nan(v.xx) || is_nan(v.xy) ||
        is_nan(v.yx) || is_nan(v.yy) ||
        is_nan(v.zx) || is_nan(v.zy)
    )

//  --------------------------------------------------------
//  GETTERS

    template <typename T>
    constexpr Vec3<T>  x_column(const Ten32<T>&v) 
    {
        return {v.xx, v.yx, v.zx};
    }

    template <typename T>
    constexpr Vec3<T>  y_column(const Ten32<T>&v) 
    {
        return {v.xy, v.yy, v.zy};
    }

    template <typename T>
    constexpr Vec2<T>  x_row(const Ten32<T>&v)
    {
        return {v.xx, v.xy};
    }

    template <typename T>
    constexpr Vec2<T>  y_row(const Ten32<T>&v)
    {
        return {v.yx, v.yy};
    }

    template <typename T>
    constexpr Vec2<T>  z_row(const Ten32<T>&v)
    {
        return {v.zx, v.zy};
    }


//  --------------------------------------------------------
//  SETTERS

    template <typename T>
    Ten32<T>& set_x_column(Ten32<T>&ten, const Vec2<T>& v)
    {
        ten.xx = v.x;
        ten.yx = v.y;
        ten.zx = v.z;
        return ten;
    }

    template <typename T>
    Ten32<T>& set_y_column(Ten32<T>&ten, const Vec2<T>& v)
    {
        ten.xy = v.x;
        ten.yy = v.y;
        ten.zy = v.z;
        return ten;
    }

    template <typename T>
    Ten32<T>& set_x_row(Ten32<T>&ten, const Vec3<T>& v)
    {
        ten.xx = v.x;
        ten.xy = v.y;
        return ten;
    }

    template <typename T>
    Ten32<T>& set_y_row(Ten32<T>&ten, const Vec3<T>& v)
    {
        ten.yx = v.x;
        ten.yy = v.y;
        return ten;
    }

    template <typename T>
    Ten32<T>& set_z_row(Ten32<T>&ten, const Vec3<T>& v)
    {
        ten.zx = v.x;
        ten.zy = v.y;
        return ten;
    }

//  --------------------------------------------------------
//  POSITIVE

    template <typename T>
    constexpr Ten32<T>  operator+(const Ten32<T>& a) 
    { 
        return a; 
    }


//  --------------------------------------------------------
//  NEGATIVE

    template <typename T>
    constexpr Ten32<T>  operator-(const Ten32<T>& a) 
    {
        return {
            -a.xx, -a.xy,
            -a.yx, -a.yy,
            -a.zx, -a.zy
        };
    }


//  --------------------------------------------------------
//  ADDITION

    template <typename T>
    constexpr Ten32<T>   operator+ (const Ten32<T> &a, const Ten32<T> &b) 
    {
        return {
            a.xx+b.xx, a.xy+b.xy,
            a.yx+b.yx, a.yy+b.yy,
            a.zx+b.zx, a.zy+b.zy
        };
    }

    template <typename T>
    Ten32<T>&   operator+=(Ten32<T> &a, const Ten32<T> &b) 
    {
        a.xx+=b.xx;  a.xy+=b.xy;
        a.yx+=b.yx;  a.yy+=b.yy;
        a.zx+=b.zx;  a.zy+=b.zy;
        return a;
    }


//  --------------------------------------------------------
//  SUBTRACTION

    template <typename T>
    constexpr Ten32<T>   operator- (const Ten32<T> &a, const Ten32<T> &b) 
    {
        return {
            a.xx-b.xx, a.xy-b.xy,
            a.yx-b.yx, a.yy-b.yy,
            a.zx-b.zx, a.zy-b.zy
        };
    }
    

    template <typename T>
    Ten32<T>&   operator-=(Ten32<T> &a, const Ten32<T> &b) 
    {
        a.xx-=b.xx;  a.xy-=b.xy;
        a.yx-=b.yx;  a.yy-=b.yy;
        a.zx-=b.zx;  a.zy-=b.zy;
        return a;
    }
    
//  --------------------------------------------------------
//  MULTIPLICATION

    template <typename T, typename U>
    requires std::is_arithmetic_v<T>
    constexpr Ten32<product_t<T,U>>  operator*(T a, const Ten32<T>& b)
    {
        return {
            a*b.xx, a*b.xy,
            a*b.yx, a*b.yy,
            a*b.zx, a*b.zy
        };
    }
    
    
    template <typename T, typename U>
    requires std::is_arithmetic_v<U>
    constexpr Ten32<product_t<T,U>>  operator*(const Ten32<T>& a, U b)
    {
        return {
            a.xx*b, a.xy*b,
            a.yx*b, a.yy*b,
            a.zx*b, a.zy*b
        };
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_mul_v<T,U>)
    Ten32<product_t<T,U>>  operator*=(const Ten32<T>& a, U b)
    {
        a.xx*=b; a.xy*=b;
        a.yx*=b; a.yy*=b;
        a.zx*=b; a.zy*=b;        
        return a;
    }

        
    template <typename T, typename U>
    constexpr Vec3<product_t<T,U>> operator*(const Ten32<T>&a, const Vec2<U>&b)
    {
        return {
            a.xx*b.x + a.xy*b.y,
            a.yx*b.x + a.yy*b.y,
            a.zx*b.x + a.zy*b.y
        };
    }

    template <typename T, typename U>
    constexpr Vec2<product_t<T,U>> operator*(const Vec3<T>&a, const Ten32<U>&b)
    {
        return {
            a.x*b.xx + a.y*b.yx + a.z*b.zx,
            a.x*b.xy + a.y*b.yy + a.z*b.zy
        };
    }

//  --------------------------------------------------------
//  DIVISION


    template <typename T, typename U>
    requires std::is_arithmetic_v<U>
    constexpr Ten32<quotient_t<T,U>>  operator/(const Ten32<T>& a, U b)
    {
        return {
            a.xx/b, a.xy/b,
            a.yx/b, a.yy/b,
            a.zx/b, a.zy/b
        };
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_div_v<T,U>)
    Ten32<quotient_t<T,U>>  operator/=(const Ten32<T>& a, U b)
    {
        a.xx/=b; a.xy/=b;
        a.yx/=b; a.yy/=b;
        a.zx/=b; a.zy/=b;        
        return a;
    }

//  --------------------------------------------------------
//  OTIMES PRODUCT

    template <typename T, typename U>
    constexpr Ten32<product_t<T,U>> operator OTIMES(const Vec3<T>&a, const Vec2<U>&b)
    {
        return {
            a.x+b.x, a.x+b.y,
            a.y+b.x, a.y+b.y,
            a.z+b.x, a.z+b.y
        };
    }

//  --------------------------------------------------------
//  ADVANCED FUNCTIONS

}

YQ_TYPE_DECLARE(yq::Ten32D)
YQ_TYPE_DECLARE(yq::Ten32F)
YQ_TYPE_DECLARE(yq::Ten32I)
YQ_TYPE_DECLARE(yq::Ten32U)

