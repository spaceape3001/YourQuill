////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

//  WARNING!  WARNING!
//
//  THIS FILE IS AUTO-GENERATED, CHANGES WILL BE CLOBBERED!

#define YQ__API__MATH__TENSOR_1_1__HPP 1
#include <math/preamble.hpp>
#include <math/vec/Vec1.hpp>

namespace yq {
    /*! \brief 1x1 second order tensor (ie a matrix)
    
        \tparam[T]  Data type to be used, recommended to be arithmetic in nature
    */
    template <typename T>
    struct Ten11 {
        using component_type = T;
        T xx;

        constexpr bool operator==(const Ten11&) const noexcept = default;
    };
        
//  --------------------------------------------------------
//  COMPOSITION

    template <typename T>
    constexpr Ten11<T>  columns(const Vec1<T>&x)
    {
        return {
            x.x
        };
    }

    template <typename T>
    constexpr Ten11<T>  diagonal(const Vec1<T>&v)
    {
        return {
            v.x
        };
    }
    
    template <typename T>
    constexpr Ten11<T>  diagonal(T x)
    {
        return {
            x
        };
    }

    template <typename T>
    constexpr Ten11<T>  rows(const Vec1<T>&x)
    {
        return {
            x.x
        };
    }
    
    YQ_IDENTITY_1(Ten11, {
        one_v<T>
    })

    YQ_NAN_1(Ten11, {
        nan_v<T> 
    })
    
    YQ_ZERO_1(Ten11, {
        zero_v<T> 
     })
    
//  --------------------------------------------------------
//  BASIC FUNCTIONS


    YQ_IS_FINITE_1(Ten11, 
        is_finite(v.xx)
    )
    
    YQ_IS_NAN_1(Ten11,  
        is_nan(v.xx)
    )

    template <typename T>
    constexpr Ten11<T>  transpose(const Ten11<T>&v)
    {
        return {
            v.xx
        };
    }

//  --------------------------------------------------------
//  GETTERS

    template <typename T>
    constexpr Vec1<T>  diagonal(const Ten11<T>& v)
    {
        return {v.xx};
    }

    template <typename T>
    constexpr Vec1<T>  x_column(const Ten11<T>&v) 
    {
        return {v.xx};
    }

    template <typename T>
    constexpr Vec1<T>  x_row(const Ten11<T>&v)
    {
        return {v.xx};
    }


//  --------------------------------------------------------
//  SETTERS

            
    template <typename T>
    Ten11<T>&  set_diagonal(Ten11<T>& ten, const Vec1<T>& v)
    {        ten.xx = v.x;
        return ten;
    }


    template <typename T>
    Ten11<T>& set_x_column(Ten11<T>&ten, const Vec1<T>& v)
    {
        ten.xx = v.x;
        return ten;
    }

    template <typename T>
    Ten11<T>& set_x_row(Ten11<T>&ten, const Vec1<T>& v)
    {
        ten.xx = v.x;
        return ten;
    }

//  --------------------------------------------------------
//  POSITIVE

    template <typename T>
    constexpr Ten11<T>  operator+(const Ten11<T>& a) 
    { 
        return a; 
    }


//  --------------------------------------------------------
//  NEGATIVE

    template <typename T>
    constexpr Ten11<T>  operator-(const Ten11<T>& a) 
    {
        return {
            -a.xx
        };
    }


//  --------------------------------------------------------
//  ADDITION

    template <typename T>
    constexpr Ten11<T>   operator+ (const Ten11<T> &a, const Ten11<T> &b) 
    {
        return {
            a.xx+b.xx
        };
    }

    template <typename T>
    Ten11<T>&   operator+=(Ten11<T> &a, const Ten11<T> &b) 
    {
        a.xx+=b.xx;
        return a;
    }


//  --------------------------------------------------------
//  SUBTRACTION

    template <typename T>
    constexpr Ten11<T>   operator- (const Ten11<T> &a, const Ten11<T> &b) 
    {
        return {
            a.xx-b.xx
        };
    }
    

    template <typename T>
    Ten11<T>&   operator-=(Ten11<T> &a, const Ten11<T> &b) 
    {
        a.xx-=b.xx;
        return a;
    }
    
//  --------------------------------------------------------
//  MULTIPLICATION

    template <typename T, typename U>
    requires std::is_arithmetic_v<T>
    constexpr Ten11<product_t<T,U>>  operator*(T a, const Ten11<T>& b)
    {
        return {
            a*b.xx
        };
    }
    
    
    template <typename T, typename U>
    requires std::is_arithmetic_v<U>
    constexpr Ten11<product_t<T,U>>  operator*(const Ten11<T>& a, U b)
    {
        return {
            a.xx*b
        };
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_mul_v<T,U>)
    Ten11<product_t<T,U>>  operator*=(const Ten11<T>& a, U b)
    {
        a.xx*=b;        
        return a;
    }

    template <typename T, typename U>
    constexpr Ten11<product_t<T,U>> operator*(const Ten11<T>& a, const Ten11<U>& b)
    {
        return {
            a.xx*b.xx
        };
    }
    
    template <typename T, typename U>
    requires trait::self_mul_v<T,U>
    Ten11<T>& operator*=(Ten11<T>&a, const Ten11<U>& b)
    {
        a = a * b;
        return a;
    }

        
    template <typename T, typename U>
    constexpr Vec1<product_t<T,U>> operator*(const Ten11<T>&a, const Vec1<U>&b)
    {
        return {
            a.xx*b.x
        };
    }

    template <typename T, typename U>
    constexpr Vec1<product_t<T,U>> operator*(const Vec1<T>&a, const Ten11<U>&b)
    {
        return {
            a.x*b.xx
        };
    }

    template <typename T, typename U>
    requires trait::self_mul_v<T,U>
    Vec1<T>& operator*=(Vec1<T>&a, const Ten11<U>& b)
    {
        a = a * b;
        return a;
    }

//  --------------------------------------------------------
//  DIVISION


    template <typename T, typename U>
    requires std::is_arithmetic_v<U>
    constexpr Ten11<quotient_t<T,U>>  operator/(const Ten11<T>& a, U b)
    {
        return {
            a.xx/b
        };
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_div_v<T,U>)
    Ten11<quotient_t<T,U>>  operator/=(const Ten11<T>& a, U b)
    {
        a.xx/=b;        
        return a;
    }

//  --------------------------------------------------------
//  OTIMES PRODUCT

    template <typename T, typename U>
    constexpr Ten11<product_t<T,U>> operator OTIMES(const Vec1<T>&a, const Vec1<U>&b)
    {
        return {
            a.x+b.x
        };
    }

//  --------------------------------------------------------
//  ADVANCED FUNCTIONS

    /*! \brief Trace of the 1 x 1 tensor
    
        \param[in] a    Tensor to take the trace of
    */
    template <typename T>
    constexpr T     trace(const Ten11<T>& a)
    {
        return a.xx;
    }

}

YQ_TYPE_DECLARE(yq::Ten11D)
YQ_TYPE_DECLARE(yq::Ten11F)
YQ_TYPE_DECLARE(yq::Ten11I)
YQ_TYPE_DECLARE(yq::Ten11U)

