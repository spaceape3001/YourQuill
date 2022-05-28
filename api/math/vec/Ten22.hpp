////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

//  WARNING!  WARNING!
//
//  THIS FILE IS AUTO-GENERATED, CHANGES WILL BE CLOBBERED!

#define YQ__API__MATH__TENSOR_2_2__HPP 1
#include <math/preamble.hpp>
#include <math/vec/Vec2.hpp>

namespace yq {
    /*! \brief 2x2 second order tensor (ie a matrix)
    
        \tparam[T]  Data type to be used, recommended to be arithmetic in nature
    */
    template <typename T>
    struct Ten22 {
        using component_type = T;
        T xx, xy;
        T yx, yy;

        constexpr bool operator==(const Ten22&) const noexcept = default;
    };
        
//  --------------------------------------------------------
//  COMPOSITION

    template <typename T>
    constexpr Ten22<T>  columns(const Vec2<T>&x, const Vec2<T>&y)
    {
        return {
            x.x, y.x,
            x.y, y.y
        };
    }

    template <typename T>
    constexpr Ten22<T>  diagonal(const Vec2<T>&v)
    {
        return {
            v.x, zero_v<T>,
            zero_v<T>, v.y
        };
    }
    
    template <typename T>
    constexpr Ten22<T>  diagonal(T x, std::type_identity_t<T> y)
    {
        return {
            x, zero_v<T>,
            zero_v<T>, y
        };
    }

    template <typename T>
    constexpr Ten22<T>  rows(const Vec2<T>&x, const Vec2<T>&y)
    {
        return {
            x.x, x.y,
            y.x, y.y
        };
    }
    
    YQ_IDENTITY_1(Ten22, {
        one_v<T>, zero_v<T>,
        zero_v<T>, one_v<T>
    })

    YQ_NAN_1(Ten22, {
        nan_v<T>, nan_v<T>,
        nan_v<T>, nan_v<T> 
    })
    
    YQ_ZERO_1(Ten22, {
        zero_v<T>, zero_v<T>,
        zero_v<T>, zero_v<T> 
     })
    
//  --------------------------------------------------------
//  BASIC FUNCTIONS


    YQ_IS_FINITE_1(Ten22, 
        is_finite(v.xx) && is_finite(v.xy) &&
        is_finite(v.yx) && is_finite(v.yy)
    )
    
    YQ_IS_NAN_1(Ten22,  
        is_nan(v.xx) || is_nan(v.xy) ||
        is_nan(v.yx) || is_nan(v.yy)
    )

    template <typename T>
    constexpr Ten22<T>  transpose(const Ten22<T>&v)
    {
        return {
            v.xx, v.yx,
            v.xy, v.yy
        };
    }

//  --------------------------------------------------------
//  GETTERS

    template <typename T>
    constexpr Vec2<T>  diagonal(const Ten22<T>& v)
    {
        return {v.xx, v.yy};
    }

    template <typename T>
    constexpr Vec2<T>  x_column(const Ten22<T>&v) 
    {
        return {v.xx, v.yx};
    }

    template <typename T>
    constexpr Vec2<T>  y_column(const Ten22<T>&v) 
    {
        return {v.xy, v.yy};
    }

    template <typename T>
    constexpr Vec2<T>  x_row(const Ten22<T>&v)
    {
        return {v.xx, v.xy};
    }

    template <typename T>
    constexpr Vec2<T>  y_row(const Ten22<T>&v)
    {
        return {v.yx, v.yy};
    }


//  --------------------------------------------------------
//  SETTERS

            
    template <typename T>
    Ten22<T>&  set_diagonal(Ten22<T>& ten, const Vec2<T>& v)
    {        ten.xx = v.x;
        ten.yy = v.y;
        return ten;
    }


    template <typename T>
    Ten22<T>& set_x_column(Ten22<T>&ten, const Vec2<T>& v)
    {
        ten.xx = v.x;
        ten.yx = v.y;
        return ten;
    }

    template <typename T>
    Ten22<T>& set_y_column(Ten22<T>&ten, const Vec2<T>& v)
    {
        ten.xy = v.x;
        ten.yy = v.y;
        return ten;
    }

    template <typename T>
    Ten22<T>& set_x_row(Ten22<T>&ten, const Vec2<T>& v)
    {
        ten.xx = v.x;
        ten.xy = v.y;
        return ten;
    }

    template <typename T>
    Ten22<T>& set_y_row(Ten22<T>&ten, const Vec2<T>& v)
    {
        ten.yx = v.x;
        ten.yy = v.y;
        return ten;
    }

//  --------------------------------------------------------
//  POSITIVE

    template <typename T>
    constexpr Ten22<T>  operator+(const Ten22<T>& a) 
    { 
        return a; 
    }


//  --------------------------------------------------------
//  NEGATIVE

    template <typename T>
    constexpr Ten22<T>  operator-(const Ten22<T>& a) 
    {
        return {
            -a.xx, -a.xy,
            -a.yx, -a.yy
        };
    }


//  --------------------------------------------------------
//  ADDITION

    template <typename T>
    constexpr Ten22<T>   operator+ (const Ten22<T> &a, const Ten22<T> &b) 
    {
        return {
            a.xx+b.xx, a.xy+b.xy,
            a.yx+b.yx, a.yy+b.yy
        };
    }

    template <typename T>
    Ten22<T>&   operator+=(Ten22<T> &a, const Ten22<T> &b) 
    {
        a.xx+=b.xx;  a.xy+=b.xy;
        a.yx+=b.yx;  a.yy+=b.yy;
        return a;
    }


//  --------------------------------------------------------
//  SUBTRACTION

    template <typename T>
    constexpr Ten22<T>   operator- (const Ten22<T> &a, const Ten22<T> &b) 
    {
        return {
            a.xx-b.xx, a.xy-b.xy,
            a.yx-b.yx, a.yy-b.yy
        };
    }
    

    template <typename T>
    Ten22<T>&   operator-=(Ten22<T> &a, const Ten22<T> &b) 
    {
        a.xx-=b.xx;  a.xy-=b.xy;
        a.yx-=b.yx;  a.yy-=b.yy;
        return a;
    }
    
//  --------------------------------------------------------
//  MULTIPLICATION

    template <typename T, typename U>
    requires std::is_arithmetic_v<T>
    constexpr Ten22<product_t<T,U>>  operator*(T a, const Ten22<T>& b)
    {
        return {
            a*b.xx, a*b.xy,
            a*b.yx, a*b.yy
        };
    }
    
    
    template <typename T, typename U>
    requires std::is_arithmetic_v<U>
    constexpr Ten22<product_t<T,U>>  operator*(const Ten22<T>& a, U b)
    {
        return {
            a.xx*b, a.xy*b,
            a.yx*b, a.yy*b
        };
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_mul_v<T,U>)
    Ten22<product_t<T,U>>  operator*=(const Ten22<T>& a, U b)
    {
        a.xx*=b; a.xy*=b;
        a.yx*=b; a.yy*=b;        
        return a;
    }

    template <typename T, typename U>
    constexpr Ten22<product_t<T,U>> operator*(const Ten22<T>& a, const Ten22<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx,
            a.xx*b.xy + a.xy*b.yy,

            a.yx*b.xx + a.yy*b.yx,
            a.yx*b.xy + a.yy*b.yy
        };
    }
    
    template <typename T, typename U>
    requires trait::self_mul_v<T,U>
    Ten22<T>& operator*=(Ten22<T>&a, const Ten22<U>& b)
    {
        a = a * b;
        return a;
    }

        
    template <typename T, typename U>
    constexpr Vec2<product_t<T,U>> operator*(const Ten22<T>&a, const Vec2<U>&b)
    {
        return {
            a.xx*b.x + a.xy*b.y,
            a.yx*b.x + a.yy*b.y
        };
    }

    template <typename T, typename U>
    constexpr Vec2<product_t<T,U>> operator*(const Vec2<T>&a, const Ten22<U>&b)
    {
        return {
            a.x*b.xx + a.y*b.yx,
            a.x*b.xy + a.y*b.yy
        };
    }

    template <typename T, typename U>
    requires trait::self_mul_v<T,U>
    Vec2<T>& operator*=(Vec2<T>&a, const Ten22<U>& b)
    {
        a = a * b;
        return a;
    }

//  --------------------------------------------------------
//  DIVISION


    template <typename T, typename U>
    requires std::is_arithmetic_v<U>
    constexpr Ten22<quotient_t<T,U>>  operator/(const Ten22<T>& a, U b)
    {
        return {
            a.xx/b, a.xy/b,
            a.yx/b, a.yy/b
        };
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_div_v<T,U>)
    Ten22<quotient_t<T,U>>  operator/=(const Ten22<T>& a, U b)
    {
        a.xx/=b; a.xy/=b;
        a.yx/=b; a.yy/=b;        
        return a;
    }

//  --------------------------------------------------------
//  OTIMES PRODUCT

    template <typename T, typename U>
    constexpr Ten22<product_t<T,U>> operator OTIMES(const Vec2<T>&a, const Vec2<U>&b)
    {
        return {
            a.x+b.x, a.x+b.y,
            a.y+b.x, a.y+b.y
        };
    }

//  --------------------------------------------------------
//  ADVANCED FUNCTIONS

    /*! \brief Trace of the 2 x 2 tensor
    
        \param[in] a    Tensor to take the trace of
    */
    template <typename T>
    constexpr T     trace(const Ten22<T>& a)
    {
        return a.xx+a.yy;
    }

}

YQ_TYPE_DECLARE(yq::Ten22D)
YQ_TYPE_DECLARE(yq::Ten22F)
YQ_TYPE_DECLARE(yq::Ten22I)
YQ_TYPE_DECLARE(yq::Ten22U)

