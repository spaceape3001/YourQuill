////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

//  WARNING!  WARNING!
//
//  THIS FILE IS AUTO-GENERATED, CHANGES WILL BE CLOBBERED!

#define YQ__API__MATH__TENSOR_3_1__HPP 1
#include <math/preamble.hpp>
#include <math/vec/Vec1.hpp>
#include <math/vec/Vec3.hpp>

namespace yq {
    /*! \brief 3x1 second order tensor (ie a matrix)
    
        \tparam[T]  Data type to be used, recommended to be arithmetic in nature
    */
    template <typename T>
    struct Ten31 {
        using component_type = T;
        T xx;
        T yx;
        T zx;

        constexpr bool operator==(const Ten31&) const noexcept = default;
    };
        
//  --------------------------------------------------------
//  COMPOSITION

    template <typename T>
    constexpr Ten31<T>  columns(const Vec3<T>&x)
    {
        return {
            x.x,
            x.y,
            x.z
        };
    }

    template <typename T>
    constexpr Ten31<T>  rows(const Vec1<T>&x, const Vec1<T>&y, const Vec1<T>&z)
    {
        return {
            x.x,
            y.x,
            z.x
        };
    }
    
    YQ_IDENTITY_1(Ten31, {
        one_v<T>,
        zero_v<T>,
        zero_v<T>
    })

    YQ_NAN_1(Ten31, {
        nan_v<T>,
        nan_v<T>,
        nan_v<T> 
    })
    
    YQ_ZERO_1(Ten31, {
        zero_v<T>,
        zero_v<T>,
        zero_v<T> 
     })
    
//  --------------------------------------------------------
//  BASIC FUNCTIONS


    YQ_IS_FINITE_1(Ten31, 
        is_finite(v.xx) &&
        is_finite(v.yx) &&
        is_finite(v.zx)
    )
    
    YQ_IS_NAN_1(Ten31,  
        is_nan(v.xx) ||
        is_nan(v.yx) ||
        is_nan(v.zx)
    )

//  --------------------------------------------------------
//  GETTERS

    template <typename T>
    constexpr Vec3<T>  x_column(const Ten31<T>&v) 
    {
        return {v.xx, v.yx, v.zx};
    }

    template <typename T>
    constexpr Vec1<T>  x_row(const Ten31<T>&v)
    {
        return {v.xx};
    }

    template <typename T>
    constexpr Vec1<T>  y_row(const Ten31<T>&v)
    {
        return {v.yx};
    }

    template <typename T>
    constexpr Vec1<T>  z_row(const Ten31<T>&v)
    {
        return {v.zx};
    }


//  --------------------------------------------------------
//  SETTERS

    template <typename T>
    Ten31<T>& set_x_column(Ten31<T>&ten, const Vec1<T>& v)
    {
        ten.xx = v.x;
        ten.yx = v.y;
        ten.zx = v.z;
        return ten;
    }

    template <typename T>
    Ten31<T>& set_x_row(Ten31<T>&ten, const Vec3<T>& v)
    {
        ten.xx = v.x;
        return ten;
    }

    template <typename T>
    Ten31<T>& set_y_row(Ten31<T>&ten, const Vec3<T>& v)
    {
        ten.yx = v.x;
        return ten;
    }

    template <typename T>
    Ten31<T>& set_z_row(Ten31<T>&ten, const Vec3<T>& v)
    {
        ten.zx = v.x;
        return ten;
    }

//  --------------------------------------------------------
//  POSITIVE

    template <typename T>
    constexpr Ten31<T>  operator+(const Ten31<T>& a) 
    { 
        return a; 
    }


//  --------------------------------------------------------
//  NEGATIVE

    template <typename T>
    constexpr Ten31<T>  operator-(const Ten31<T>& a) 
    {
        return {
            -a.xx,
            -a.yx,
            -a.zx
        };
    }


//  --------------------------------------------------------
//  ADDITION

    template <typename T>
    constexpr Ten31<T>   operator+ (const Ten31<T> &a, const Ten31<T> &b) 
    {
        return {
            a.xx+b.xx,
            a.yx+b.yx,
            a.zx+b.zx
        };
    }

    template <typename T>
    Ten31<T>&   operator+=(Ten31<T> &a, const Ten31<T> &b) 
    {
        a.xx+=b.xx;
        a.yx+=b.yx;
        a.zx+=b.zx;
        return a;
    }


//  --------------------------------------------------------
//  SUBTRACTION

    template <typename T>
    constexpr Ten31<T>   operator- (const Ten31<T> &a, const Ten31<T> &b) 
    {
        return {
            a.xx-b.xx,
            a.yx-b.yx,
            a.zx-b.zx
        };
    }
    

    template <typename T>
    Ten31<T>&   operator-=(Ten31<T> &a, const Ten31<T> &b) 
    {
        a.xx-=b.xx;
        a.yx-=b.yx;
        a.zx-=b.zx;
        return a;
    }
    
//  --------------------------------------------------------
//  MULTIPLICATION

    template <typename T, typename U>
    requires std::is_arithmetic_v<T>
    constexpr Ten31<product_t<T,U>>  operator*(T a, const Ten31<T>& b)
    {
        return {
            a*b.xx,
            a*b.yx,
            a*b.zx
        };
    }
    
    
    template <typename T, typename U>
    requires std::is_arithmetic_v<U>
    constexpr Ten31<product_t<T,U>>  operator*(const Ten31<T>& a, U b)
    {
        return {
            a.xx*b,
            a.yx*b,
            a.zx*b
        };
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_mul_v<T,U>)
    Ten31<product_t<T,U>>  operator*=(const Ten31<T>& a, U b)
    {
        a.xx*=b;
        a.yx*=b;
        a.zx*=b;        
        return a;
    }

        
    template <typename T, typename U>
    constexpr Vec3<product_t<T,U>> operator*(const Ten31<T>&a, const Vec1<U>&b)
    {
        return {
            a.xx*b.x,
            a.yx*b.x,
            a.zx*b.x
        };
    }

    template <typename T, typename U>
    constexpr Vec1<product_t<T,U>> operator*(const Vec3<T>&a, const Ten31<U>&b)
    {
        return {
            a.x*b.xx + a.y*b.yx + a.z*b.zx
        };
    }

//  --------------------------------------------------------
//  DIVISION


    template <typename T, typename U>
    requires std::is_arithmetic_v<U>
    constexpr Ten31<quotient_t<T,U>>  operator/(const Ten31<T>& a, U b)
    {
        return {
            a.xx/b,
            a.yx/b,
            a.zx/b
        };
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_div_v<T,U>)
    Ten31<quotient_t<T,U>>  operator/=(const Ten31<T>& a, U b)
    {
        a.xx/=b;
        a.yx/=b;
        a.zx/=b;        
        return a;
    }

//  --------------------------------------------------------
//  OTIMES PRODUCT

    template <typename T, typename U>
    constexpr Ten31<product_t<T,U>> operator OTIMES(const Vec3<T>&a, const Vec1<U>&b)
    {
        return {
            a.x+b.x,
            a.y+b.x,
            a.z+b.x
        };
    }

//  --------------------------------------------------------
//  ADVANCED FUNCTIONS

}

YQ_TYPE_DECLARE(yq::Ten31D)
YQ_TYPE_DECLARE(yq::Ten31F)
YQ_TYPE_DECLARE(yq::Ten31I)
YQ_TYPE_DECLARE(yq::Ten31U)

