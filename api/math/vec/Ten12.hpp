////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

//  WARNING!  WARNING!
//
//  THIS FILE IS AUTO-GENERATED, CHANGES WILL BE CLOBBERED!

#define YQ__API__MATH__TENSOR_1_2__HPP 1
#include <math/preamble.hpp>
#include <math/vec/Vec1.hpp>
#include <math/vec/Vec2.hpp>

namespace yq {
    /*! \brief 1x2 second order tensor (ie a matrix)
    
        \tparam[T]  Data type to be used, recommended to be arithmetic in nature
    */
    template <typename T>
    struct Ten12 {
        using component_type = T;
        T xx, xy;

        constexpr bool operator==(const Ten12&) const noexcept = default;
    };
        
//  --------------------------------------------------------
//  COMPOSITION

    template <typename T>
    constexpr Ten12<T>  columns(const Vec1<T>&x, const Vec1<T>&y)
    {
        return {
            x.x, y.x
        };
    }

    template <typename T>
    constexpr Ten12<T>  rows(const Vec2<T>&x)
    {
        return {
            x.x, x.y
        };
    }
    
    YQ_IDENTITY_1(Ten12, {
        one_v<T>, zero_v<T>
    })

    YQ_NAN_1(Ten12, {
        nan_v<T>, nan_v<T> 
    })
    
    YQ_ZERO_1(Ten12, {
        zero_v<T>, zero_v<T> 
     })
    
//  --------------------------------------------------------
//  BASIC FUNCTIONS


    YQ_IS_FINITE_1(Ten12, 
        is_finite(v.xx) && is_finite(v.xy)
    )
    
    YQ_IS_NAN_1(Ten12,  
        is_nan(v.xx) || is_nan(v.xy)
    )

//  --------------------------------------------------------
//  GETTERS

    template <typename T>
    constexpr Vec1<T>  x_column(const Ten12<T>&v) 
    {
        return {v.xx};
    }

    template <typename T>
    constexpr Vec1<T>  y_column(const Ten12<T>&v) 
    {
        return {v.xy};
    }

    template <typename T>
    constexpr Vec2<T>  x_row(const Ten12<T>&v)
    {
        return {v.xx, v.xy};
    }


//  --------------------------------------------------------
//  SETTERS

    template <typename T>
    Ten12<T>& set_x_column(Ten12<T>&ten, const Vec2<T>& v)
    {
        ten.xx = v.x;
        return ten;
    }

    template <typename T>
    Ten12<T>& set_y_column(Ten12<T>&ten, const Vec2<T>& v)
    {
        ten.xy = v.x;
        return ten;
    }

    template <typename T>
    Ten12<T>& set_x_row(Ten12<T>&ten, const Vec1<T>& v)
    {
        ten.xx = v.x;
        ten.xy = v.y;
        return ten;
    }

//  --------------------------------------------------------
//  POSITIVE

    template <typename T>
    constexpr Ten12<T>  operator+(const Ten12<T>& a) 
    { 
        return a; 
    }


//  --------------------------------------------------------
//  NEGATIVE

    template <typename T>
    constexpr Ten12<T>  operator-(const Ten12<T>& a) 
    {
        return {
            -a.xx, -a.xy
        };
    }


//  --------------------------------------------------------
//  ADDITION

    template <typename T>
    constexpr Ten12<T>   operator+ (const Ten12<T> &a, const Ten12<T> &b) 
    {
        return {
            a.xx+b.xx, a.xy+b.xy
        };
    }

    template <typename T>
    Ten12<T>&   operator+=(Ten12<T> &a, const Ten12<T> &b) 
    {
        a.xx+=b.xx;  a.xy+=b.xy;
        return a;
    }


//  --------------------------------------------------------
//  SUBTRACTION

    template <typename T>
    constexpr Ten12<T>   operator- (const Ten12<T> &a, const Ten12<T> &b) 
    {
        return {
            a.xx-b.xx, a.xy-b.xy
        };
    }
    

    template <typename T>
    Ten12<T>&   operator-=(Ten12<T> &a, const Ten12<T> &b) 
    {
        a.xx-=b.xx;  a.xy-=b.xy;
        return a;
    }
    
//  --------------------------------------------------------
//  MULTIPLICATION

    template <typename T, typename U>
    requires std::is_arithmetic_v<T>
    constexpr Ten12<product_t<T,U>>  operator*(T a, const Ten12<T>& b)
    {
        return {
            a*b.xx, a*b.xy
        };
    }
    
    
    template <typename T, typename U>
    requires std::is_arithmetic_v<U>
    constexpr Ten12<product_t<T,U>>  operator*(const Ten12<T>& a, U b)
    {
        return {
            a.xx*b, a.xy*b
        };
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_mul_v<T,U>)
    Ten12<product_t<T,U>>  operator*=(const Ten12<T>& a, U b)
    {
        a.xx*=b; a.xy*=b;        
        return a;
    }

        
    template <typename T, typename U>
    constexpr Vec1<product_t<T,U>> operator*(const Ten12<T>&a, const Vec2<U>&b)
    {
        return {
            a.xx*b.x + a.xy*b.y
        };
    }

    template <typename T, typename U>
    constexpr Vec2<product_t<T,U>> operator*(const Vec1<T>&a, const Ten12<U>&b)
    {
        return {
            a.x*b.xx,
            a.x*b.xy
        };
    }

//  --------------------------------------------------------
//  DIVISION


    template <typename T, typename U>
    requires std::is_arithmetic_v<U>
    constexpr Ten12<quotient_t<T,U>>  operator/(const Ten12<T>& a, U b)
    {
        return {
            a.xx/b, a.xy/b
        };
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_div_v<T,U>)
    Ten12<quotient_t<T,U>>  operator/=(const Ten12<T>& a, U b)
    {
        a.xx/=b; a.xy/=b;        
        return a;
    }

//  --------------------------------------------------------
//  OTIMES PRODUCT

    template <typename T, typename U>
    constexpr Ten12<product_t<T,U>> operator OTIMES(const Vec1<T>&a, const Vec2<U>&b)
    {
        return {
            a.x+b.x, a.x+b.y
        };
    }

//  --------------------------------------------------------
//  ADVANCED FUNCTIONS

}

YQ_TYPE_DECLARE(yq::Ten12D)
YQ_TYPE_DECLARE(yq::Ten12F)
YQ_TYPE_DECLARE(yq::Ten12I)
YQ_TYPE_DECLARE(yq::Ten12U)

