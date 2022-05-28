////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

//  WARNING!  WARNING!
//
//  THIS FILE IS AUTO-GENERATED, CHANGES WILL BE CLOBBERED!

#define YQ__API__MATH__TENSOR_2_3__HPP 1
#include <math/preamble.hpp>
#include <math/vec/Vec2.hpp>
#include <math/vec/Vec3.hpp>

namespace yq {
    /*! \brief 2x3 second order tensor (ie a matrix)
    
        \tparam[T]  Data type to be used, recommended to be arithmetic in nature
    */
    template <typename T>
    struct Ten23 {
        using component_type = T;
        T xx, xy, xz;
        T yx, yy, yz;

        constexpr bool operator==(const Ten23&) const noexcept = default;
    };
        
//  --------------------------------------------------------
//  COMPOSITION

    template <typename T>
    constexpr Ten23<T>  columns(const Vec2<T>&x, const Vec2<T>&y, const Vec2<T>&z)
    {
        return {
            x.x, y.x, z.x,
            x.y, y.y, z.y
        };
    }

    template <typename T>
    constexpr Ten23<T>  rows(const Vec3<T>&x, const Vec3<T>&y)
    {
        return {
            x.x, x.y, x.z,
            y.x, y.y, y.z
        };
    }
    
    YQ_IDENTITY_1(Ten23, {
        one_v<T>, zero_v<T>, zero_v<T>,
        zero_v<T>, one_v<T>, zero_v<T>
    })

    YQ_NAN_1(Ten23, {
        nan_v<T>, nan_v<T>, nan_v<T>,
        nan_v<T>, nan_v<T>, nan_v<T> 
    })
    
    YQ_ZERO_1(Ten23, {
        zero_v<T>, zero_v<T>, zero_v<T>,
        zero_v<T>, zero_v<T>, zero_v<T> 
     })
    
//  --------------------------------------------------------
//  BASIC FUNCTIONS


    YQ_IS_FINITE_1(Ten23, 
        is_finite(v.xx) && is_finite(v.xy) && is_finite(v.xz) &&
        is_finite(v.yx) && is_finite(v.yy) && is_finite(v.yz)
    )
    
    YQ_IS_NAN_1(Ten23,  
        is_nan(v.xx) || is_nan(v.xy) || is_nan(v.xz) ||
        is_nan(v.yx) || is_nan(v.yy) || is_nan(v.yz)
    )

//  --------------------------------------------------------
//  GETTERS

    template <typename T>
    constexpr Vec2<T>  x_column(const Ten23<T>&v) 
    {
        return {v.xx, v.yx};
    }

    template <typename T>
    constexpr Vec2<T>  y_column(const Ten23<T>&v) 
    {
        return {v.xy, v.yy};
    }

    template <typename T>
    constexpr Vec2<T>  z_column(const Ten23<T>&v) 
    {
        return {v.xz, v.yz};
    }

    template <typename T>
    constexpr Vec3<T>  x_row(const Ten23<T>&v)
    {
        return {v.xx, v.xy, v.xz};
    }

    template <typename T>
    constexpr Vec3<T>  y_row(const Ten23<T>&v)
    {
        return {v.yx, v.yy, v.yz};
    }


//  --------------------------------------------------------
//  SETTERS

    template <typename T>
    Ten23<T>& set_x_column(Ten23<T>&ten, const Vec3<T>& v)
    {
        ten.xx = v.x;
        ten.yx = v.y;
        return ten;
    }

    template <typename T>
    Ten23<T>& set_y_column(Ten23<T>&ten, const Vec3<T>& v)
    {
        ten.xy = v.x;
        ten.yy = v.y;
        return ten;
    }

    template <typename T>
    Ten23<T>& set_z_column(Ten23<T>&ten, const Vec3<T>& v)
    {
        ten.xz = v.x;
        ten.yz = v.y;
        return ten;
    }

    template <typename T>
    Ten23<T>& set_x_row(Ten23<T>&ten, const Vec2<T>& v)
    {
        ten.xx = v.x;
        ten.xy = v.y;
        ten.xz = v.z;
        return ten;
    }

    template <typename T>
    Ten23<T>& set_y_row(Ten23<T>&ten, const Vec2<T>& v)
    {
        ten.yx = v.x;
        ten.yy = v.y;
        ten.yz = v.z;
        return ten;
    }

//  --------------------------------------------------------
//  POSITIVE

    template <typename T>
    constexpr Ten23<T>  operator+(const Ten23<T>& a) 
    { 
        return a; 
    }


//  --------------------------------------------------------
//  NEGATIVE

    template <typename T>
    constexpr Ten23<T>  operator-(const Ten23<T>& a) 
    {
        return {
            -a.xx, -a.xy, -a.xz,
            -a.yx, -a.yy, -a.yz
        };
    }


//  --------------------------------------------------------
//  ADDITION

    template <typename T>
    constexpr Ten23<T>   operator+ (const Ten23<T> &a, const Ten23<T> &b) 
    {
        return {
            a.xx+b.xx, a.xy+b.xy, a.xz+b.xz,
            a.yx+b.yx, a.yy+b.yy, a.yz+b.yz
        };
    }

    template <typename T>
    Ten23<T>&   operator+=(Ten23<T> &a, const Ten23<T> &b) 
    {
        a.xx+=b.xx;  a.xy+=b.xy;  a.xz+=b.xz;
        a.yx+=b.yx;  a.yy+=b.yy;  a.yz+=b.yz;
        return a;
    }


//  --------------------------------------------------------
//  SUBTRACTION

    template <typename T>
    constexpr Ten23<T>   operator- (const Ten23<T> &a, const Ten23<T> &b) 
    {
        return {
            a.xx-b.xx, a.xy-b.xy, a.xz-b.xz,
            a.yx-b.yx, a.yy-b.yy, a.yz-b.yz
        };
    }
    

    template <typename T>
    Ten23<T>&   operator-=(Ten23<T> &a, const Ten23<T> &b) 
    {
        a.xx-=b.xx;  a.xy-=b.xy;  a.xz-=b.xz;
        a.yx-=b.yx;  a.yy-=b.yy;  a.yz-=b.yz;
        return a;
    }
    
//  --------------------------------------------------------
//  MULTIPLICATION

    template <typename T, typename U>
    requires std::is_arithmetic_v<T>
    constexpr Ten23<product_t<T,U>>  operator*(T a, const Ten23<T>& b)
    {
        return {
            a*b.xx, a*b.xy, a*b.xz,
            a*b.yx, a*b.yy, a*b.yz
        };
    }
    
    
    template <typename T, typename U>
    requires std::is_arithmetic_v<U>
    constexpr Ten23<product_t<T,U>>  operator*(const Ten23<T>& a, U b)
    {
        return {
            a.xx*b, a.xy*b, a.xz*b,
            a.yx*b, a.yy*b, a.yz*b
        };
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_mul_v<T,U>)
    Ten23<product_t<T,U>>  operator*=(const Ten23<T>& a, U b)
    {
        a.xx*=b; a.xy*=b; a.xz*=b;
        a.yx*=b; a.yy*=b; a.yz*=b;        
        return a;
    }

        
    template <typename T, typename U>
    constexpr Vec2<product_t<T,U>> operator*(const Ten23<T>&a, const Vec3<U>&b)
    {
        return {
            a.xx*b.x + a.xy*b.y + a.xz*b.z,
            a.yx*b.x + a.yy*b.y + a.yz*b.z
        };
    }

    template <typename T, typename U>
    constexpr Vec3<product_t<T,U>> operator*(const Vec2<T>&a, const Ten23<U>&b)
    {
        return {
            a.x*b.xx + a.y*b.yx,
            a.x*b.xy + a.y*b.yy,
            a.x*b.xz + a.y*b.yz
        };
    }

//  --------------------------------------------------------
//  DIVISION


    template <typename T, typename U>
    requires std::is_arithmetic_v<U>
    constexpr Ten23<quotient_t<T,U>>  operator/(const Ten23<T>& a, U b)
    {
        return {
            a.xx/b, a.xy/b, a.xz/b,
            a.yx/b, a.yy/b, a.yz/b
        };
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_div_v<T,U>)
    Ten23<quotient_t<T,U>>  operator/=(const Ten23<T>& a, U b)
    {
        a.xx/=b; a.xy/=b; a.xz/=b;
        a.yx/=b; a.yy/=b; a.yz/=b;        
        return a;
    }

//  --------------------------------------------------------
//  OTIMES PRODUCT

    template <typename T, typename U>
    constexpr Ten23<product_t<T,U>> operator OTIMES(const Vec2<T>&a, const Vec3<U>&b)
    {
        return {
            a.x+b.x, a.x+b.y, a.x+b.z,
            a.y+b.x, a.y+b.y, a.y+b.z
        };
    }

//  --------------------------------------------------------
//  ADVANCED FUNCTIONS

}

YQ_TYPE_DECLARE(yq::Ten23D)
YQ_TYPE_DECLARE(yq::Ten23F)
YQ_TYPE_DECLARE(yq::Ten23I)
YQ_TYPE_DECLARE(yq::Ten23U)

