////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

//  WARNING!  WARNING!
//
//  THIS FILE IS AUTO-GENERATED, CHANGES WILL BE CLOBBERED!

#define YQ__API__MATH__TENSOR_2_1__HPP 1
#include <math/preamble.hpp>
#include <math/vec/Vec1.hpp>
#include <math/vec/Vec2.hpp>

namespace yq {
    /*! \brief 2x1 second order tensor (ie a matrix)
    
        \tparam[T]  Data type to be used, recommended to be arithmetic in nature
    */
    template <typename T>
    struct Ten21 {
        using component_type = T;
        T xx;
        T yx;

        constexpr bool operator==(const Ten21&) const noexcept = default;
    };
        
//  --------------------------------------------------------
//  COMPOSITION

    template <typename T>
    constexpr Ten21<T>  columns(const Vec2<T>&x)
    {
        return {
            x.x,
            x.y
        };
    }

    template <typename T>
    constexpr Ten21<T>  rows(const Vec1<T>&x, const Vec1<T>&y)
    {
        return {
            x.x,
            y.x
        };
    }
    
    YQ_IDENTITY_1(Ten21, {
        one_v<T>,
        zero_v<T>
    })

    YQ_NAN_1(Ten21, {
        nan_v<T>,
        nan_v<T> 
    })
    
    YQ_ZERO_1(Ten21, {
        zero_v<T>,
        zero_v<T> 
     })
    
//  --------------------------------------------------------
//  BASIC FUNCTIONS


    YQ_IS_FINITE_1(Ten21, 
        is_finite(v.xx) &&
        is_finite(v.yx)
    )
    
    YQ_IS_NAN_1(Ten21,  
        is_nan(v.xx) ||
        is_nan(v.yx)
    )

//  --------------------------------------------------------
//  GETTERS

    template <typename T>
    constexpr Vec2<T>  x_column(const Ten21<T>&v) 
    {
        return {v.xx, v.yx};
    }

    template <typename T>
    constexpr Vec1<T>  x_row(const Ten21<T>&v)
    {
        return {v.xx};
    }

    template <typename T>
    constexpr Vec1<T>  y_row(const Ten21<T>&v)
    {
        return {v.yx};
    }


//  --------------------------------------------------------
//  SETTERS

    template <typename T>
    Ten21<T>& set_x_column(Ten21<T>&ten, const Vec1<T>& v)
    {
        ten.xx = v.x;
        ten.yx = v.y;
        return ten;
    }

    template <typename T>
    Ten21<T>& set_x_row(Ten21<T>&ten, const Vec2<T>& v)
    {
        ten.xx = v.x;
        return ten;
    }

    template <typename T>
    Ten21<T>& set_y_row(Ten21<T>&ten, const Vec2<T>& v)
    {
        ten.yx = v.x;
        return ten;
    }

//  --------------------------------------------------------
//  POSITIVE

    template <typename T>
    constexpr Ten21<T>  operator+(const Ten21<T>& a) 
    { 
        return a; 
    }


//  --------------------------------------------------------
//  NEGATIVE

    template <typename T>
    constexpr Ten21<T>  operator-(const Ten21<T>& a) 
    {
        return {
            -a.xx,
            -a.yx
        };
    }


//  --------------------------------------------------------
//  ADDITION

    template <typename T>
    constexpr Ten21<T>   operator+ (const Ten21<T> &a, const Ten21<T> &b) 
    {
        return {
            a.xx+b.xx,
            a.yx+b.yx
        };
    }

    template <typename T>
    Ten21<T>&   operator+=(Ten21<T> &a, const Ten21<T> &b) 
    {
        a.xx+=b.xx;
        a.yx+=b.yx;
        return a;
    }


//  --------------------------------------------------------
//  SUBTRACTION

    template <typename T>
    constexpr Ten21<T>   operator- (const Ten21<T> &a, const Ten21<T> &b) 
    {
        return {
            a.xx-b.xx,
            a.yx-b.yx
        };
    }
    

    template <typename T>
    Ten21<T>&   operator-=(Ten21<T> &a, const Ten21<T> &b) 
    {
        a.xx-=b.xx;
        a.yx-=b.yx;
        return a;
    }
    
//  --------------------------------------------------------
//  MULTIPLICATION

    template <typename T, typename U>
    requires std::is_arithmetic_v<T>
    constexpr Ten21<product_t<T,U>>  operator*(T a, const Ten21<T>& b)
    {
        return {
            a*b.xx,
            a*b.yx
        };
    }
    
    
    template <typename T, typename U>
    requires std::is_arithmetic_v<U>
    constexpr Ten21<product_t<T,U>>  operator*(const Ten21<T>& a, U b)
    {
        return {
            a.xx*b,
            a.yx*b
        };
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_mul_v<T,U>)
    Ten21<product_t<T,U>>  operator*=(const Ten21<T>& a, U b)
    {
        a.xx*=b;
        a.yx*=b;        
        return a;
    }

        
    template <typename T, typename U>
    constexpr Vec2<product_t<T,U>> operator*(const Ten21<T>&a, const Vec1<U>&b)
    {
        return {
            a.xx*b.x,
            a.yx*b.x
        };
    }

    template <typename T, typename U>
    constexpr Vec1<product_t<T,U>> operator*(const Vec2<T>&a, const Ten21<U>&b)
    {
        return {
            a.x*b.xx + a.y*b.yx
        };
    }

//  --------------------------------------------------------
//  DIVISION


    template <typename T, typename U>
    requires std::is_arithmetic_v<U>
    constexpr Ten21<quotient_t<T,U>>  operator/(const Ten21<T>& a, U b)
    {
        return {
            a.xx/b,
            a.yx/b
        };
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_div_v<T,U>)
    Ten21<quotient_t<T,U>>  operator/=(const Ten21<T>& a, U b)
    {
        a.xx/=b;
        a.yx/=b;        
        return a;
    }

//  --------------------------------------------------------
//  OTIMES PRODUCT

    template <typename T, typename U>
    constexpr Ten21<product_t<T,U>> operator OTIMES(const Vec2<T>&a, const Vec1<U>&b)
    {
        return {
            a.x+b.x,
            a.y+b.x
        };
    }

//  --------------------------------------------------------
//  ADVANCED FUNCTIONS

}

YQ_TYPE_DECLARE(yq::Ten21D)
YQ_TYPE_DECLARE(yq::Ten21F)
YQ_TYPE_DECLARE(yq::Ten21I)
YQ_TYPE_DECLARE(yq::Ten21U)

