////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math/preamble.hpp>
#include "UDimension.hpp"

namespace yq {
    template <typename T, typename DIM>
    struct UMks {
        using dimension         = DIM;
        using component_type    = T;
    
        T       value = {};
        
        template <int N>
        requires std::is_floating_point_v<T>
        auto power() const;
        template <int N>
        requires std::is_floating_point_v<T>
        auto root() const;
        template <int N,int D=1>
        requires std::is_floating_point_v<T>
        auto to() const;
        
        constexpr auto operator<=>(const UMks&) const noexcept = default;
        
        
        
    };
    
    template<typename T, typename DIM> 
    struct ieee754<UMks<T,DIM>> : public ieee754<T> {
    };

    template <typename T, typename DIM>
        template <int N>
        requires std::is_floating_point_v<T>
    auto UMks<T,DIM>::power() const
    {
        return UMks<T, typename DIM::template _pow_<N,1>>{::pow(value, (double)N)};
    }
    
    template <typename T, typename DIM>
        template <int D>
        requires std::is_floating_point_v<T>
    auto UMks<T,DIM>::root() const
    {
        return UMks<T,typename DIM::template _pow_<1,D>>{::pow(value, 1.0 / (double) D)};
    }
    

    template <typename T, typename DIM>
        template <int N,int D>
        requires std::is_floating_point_v<T>
    auto UMks<T,DIM>::to() const
    {
        return UMks<T,typename DIM::template _pow_<N,D>>(::pow(value, (double) N /  (double) D));
    }
    
//  --------------------------------------------------------
//  COMPOSITION

    template <typename T, typename DIM>
    UMks<T,DIM> mks(const UMks<T,DIM>& v)
    {
        return v;
    }

    template <typename T, typename DIM>
    struct nan_eval<UMks<T,DIM>> : public std::true_type {
        static consteval UMks<T,DIM> make()
        {                                                   
            return { nan_v<T> };
        }                                                   
    };

    template <typename T, typename DIM>
    struct zero_eval<UMks<T,DIM>> : public std::true_type {
        static consteval UMks<T,DIM> make()
        {
            return { T{} };
        }
    };
    
//  --------------------------------------------------------
//  GETTERS

//  --------------------------------------------------------
//  SETTERS

//  --------------------------------------------------------
//  BASIC FUNCTIONS

    template <typename T, typename DIM>
    UMks<T,DIM> abs(const UMks<T,DIM>& v)
    {
        return { abs(v.value) };
    }
    
    template <typename T, typename DIM>
    requires std::is_floating_point_v<T>
    auto cbrt(const UMks<T,DIM>& v)
    {
        return UMks<T,typename DIM::template _pow_<1,3>>{cbrt(v.value)};
    }

    template <typename T, typename DIM>
    auto cube(const UMks<T,DIM>& v)
    {
        return UMks<cube_t<T>, typename DIM::template _pow_<3,1>>{ v.value * v.value * v.value };
    }

    template <typename T, typename DIM>
    bool is_finite(const UMks<T,DIM>& v)
    {
        return is_finite(v.value);
    }

    template <typename T, typename DIM>
    bool is_nan(const UMks<T,DIM>& v)
    {
        return is_nan(v.value);
    }
    
    template <typename T, typename DIM>
    requires trait::has_sqrt_v<T>
    auto sqrt(const UMks<T,DIM>& v)
    {
        auto ret    = sqrt(v.value);
        return UMks<decltype(ret), typename DIM::template _pow_<1,2>>{ ret };
    }

    template <typename T, typename DIM>
    auto square(const UMks<T,DIM>& v)
    {
        return UMks<square_t<T>, typename DIM::template _pow_<2,1>>{ v.value * v.value };
    }


//  --------------------------------------------------------
//  POSITIVE

    template <typename T, typename DIM>
    UMks<T,DIM> operator+(const UMks<T,DIM>& a)
    {
        return a;
    }


//  --------------------------------------------------------
//  NEGATIVE

    template <typename T, typename DIM>
    UMks<T,DIM> operator-(const UMks<T,DIM>& a)
    {
        return { -a.value };
    }

//  --------------------------------------------------------
//  NORMALIZATION


//  --------------------------------------------------------
//  ADDITION

    template <typename T, typename DIM>
    UMks<T,DIM> operator+(const UMks<T,DIM>& a, const UMks<T,DIM>& b)
    {
        return { a.value + b.value };
    }

    template <typename T, typename DIM>
    UMks<T,DIM>& operator+=(UMks<T,DIM>& a, const UMks<T,DIM>& b)
    {
        a.value += b.value;
        return a;
    }

//  --------------------------------------------------------
//  SUBTRACTION

    template <typename T, typename DIM>
    UMks<T,DIM> operator-(const UMks<T,DIM>& a, const UMks<T,DIM>& b)
    {
        return { a.value - b.value };
    }

    template <typename T, typename DIM>
    UMks<T,DIM>& operator-=(UMks<T,DIM>& a, const UMks<T,DIM>& b)
    {
        a.value -= b.value;
        return a;
    }


//  --------------------------------------------------------
//  MULTIPLICATION

    template <typename T, typename DIM>
    requires trait::is_basic_v<T>
    UMks<T,DIM> operator*(T a, const UMks<T,DIM>& b)
    {
        return { a * b.value };
    }
    
    template <typename T, typename DIM>
    requires trait::is_basic_v<T>
    UMks<T,DIM> operator*(const UMks<T,DIM>& a, T  b)
    {
        return { a.value * b };
    }
    
    template <typename T, typename DIM>
    requires trait::is_basic_v<T>
    UMks<T,DIM>& operator*=(UMks<T,DIM>& a, T  b)
    {
        a.value *= b;
        return a;
    }

    template <typename T, typename U, typename DIMT, typename DIMU>
    UMks<product_t<T,U>,typename DIMT::template _mult_<DIMU>> operator*(const UMks<T,DIMT>& a, const UMks<U,DIMU>& b)
    {
        return { a.value * b.value };
    }
    

//  --------------------------------------------------------
//  DIVISION

    template <typename T, typename DIM>
    requires trait::is_basic_v<T>
    UMks<T,typename DIM::template _pow_<-1,1>> operator/(T a, const UMks<T,DIM>& b)
    {
        return { a / b.value };
    }
    
    template <typename T, typename DIM>
    requires trait::is_basic_v<T>
    UMks<T,DIM> operator/(const UMks<T,DIM>& a, T  b)
    {
        return { a.value / b };
    }
    
    template <typename T, typename DIM>
    requires trait::is_basic_v<T>
    UMks<T,DIM>& operator/=(UMks<T,DIM>& a, T  b)
    {
        a.value /= b;
        return a;
    }

    template <typename T, typename U, typename DIMT, typename DIMU>
    UMks<quotient_t<T,U>,typename DIMT::template _div_<DIMU>> operator*(const UMks<T,DIMT>& a, const UMks<U,DIMU>& b)
    {
        return { a.value / b.value };
    }

//  --------------------------------------------------------
//  DOT PRODUCT


//  --------------------------------------------------------
//  INNER PRODUCT


//  --------------------------------------------------------
//  OUTER PRODUCT


///  --------------------------------------------------------
//  OTIMES PRODUCT

//  --------------------------------------------------------
//  PROJECTIONS

//  --------------------------------------------------------
//  ADVANCED FUNCTIONS


//  --------------------------------------------------------
//  CONDITIONAL INCLUDES
    
}
