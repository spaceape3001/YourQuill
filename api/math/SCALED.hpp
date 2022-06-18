#pragma once
#include "UMks.hpp"

namespace yq {
    template <typename T, typename DIM, double K>
    struct UScalar {
        using dimension         = DIM;
        using component_type    = T;
        static constexpr const double   FACTOR = K;
        
        T       value = {};
        
        auto operator<=>(const UScalar& b) const noexcept = default;
        
        operator UMks<T,DIM>() const noexcept { return { T(value*K) }; }
        
        UScalar&    operator=(const UMks<T,DIM>& v) 
        {
            value   = T(v / K);
            return *this;
        }
    };

//  --------------------------------------------------------
//  COMPOSITION

    template <typename T, typename DIM, double K>
    UMks<T,DIM> mks(const UScalar<T,DIM,K>& v)
    {
        return (UMks<T,DIM>) v;
    }

    template <typename T, typename DIM, double K>
    struct nan_eval<UScalar<T,DIM,K>> : public std::true_type {
        static consteval UScalar<T,DIM,K> make()
        {                                                   
            return { nan_v<T> };
        }                                                   
    };

    template <typename T, typename DIM, double K>
    struct zero_eval<UScalar<T,DIM,K>> : public std::true_type {
        static consteval UScalar<T,DIM,K> make()
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

    template <typename T, typename DIM, double K>
    bool is_finite(const UScalar<T,DIM,K>& v)
    {
        return is_finite(v.value);
    }

    template <typename T, typename DIM, double K>
    bool is_nan(const UScalar<T,DIM,K>& v)
    {
        return is_nan(v.value);
    }

//  --------------------------------------------------------
//  POSITIVE

    template <typename T, typename DIM, double K>
    UScalar<T,DIM,K> operator+(const UScalar<T,DIM,K>& a)
    {
        return a;
    }


//  --------------------------------------------------------
//  NEGATIVE

    template <typename T, typename DIM, double K>
    UScalar<T,DIM,K> operator-(const UScalar<T,DIM,K>& a)
    {
        return { -a.value };
    }


}
