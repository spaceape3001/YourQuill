////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <math/preamble.hpp>
#include <math/vec/Vec4.hpp>

namespace yq {

    /*! \brief Axially aligned box in 4 dimension(s).
    */
    template <typename T>
    struct AxBox4 {
        using component_t   = T;
        
        Vec4<T>  lo, hi;
        
        constexpr bool operator==(const AxBox4&) const noexcept = default;
    };
    
//  --------------------------------------------------------
//  COMPOSITION

    template <typename T>
    constexpr AxBox4<T> aabb(const Vec4<T>& a, const Vec4<T>& b)
    {
        return { min_elem(a,b), max_elem(a,b) };
    }
    
    YQ_NAN_1(AxBox4, { nan_v<Vec4>, nan_v<Vec4>});
    YQ_ZERO_1(AxBox4, { zero_v<Vec4>, zero_v<Vec4>});

//  --------------------------------------------------------
//  BASIC FUNCTIONS

    template <typename T>
    bool    valid(const AxBox4<T>& a)
    {
        return all_lees_equal(a.lo, a.hi);
    }

    YQ_IS_FINITE_1( AxBox4, is_finite(v.lo) && is_finite(v.hi))
    YQ_IS_NAN_1(AxBox4, is_nan(v.lo) || is_nan(v.hi))

//  --------------------------------------------------------
//  OPERATIONS

    /*! \brief Union of two AABBs
    */
    template <typename T>
    constexpr AxBox4<T> operator|(const AxBox4<T>&a, const AxBox4<T>&b)
    {
        return { min_elem(a.lo, b.lo), max_elem(a.hi, b.hi) };
    }

    /*! \brief Intersection of two AABBs
    */
    template <typename T>
    constexpr AxBox4<T> operator&(const AxBox4<T>&a, const AxBox4<T>&b)
    {
        return { max_elem(a.lo, b.lo), min_elem(a.hi, b.hi) };
    }

//  --------------------------------------------------------
//  ADVANCED FUNCTIONS
    
    template <typename T>
    constexpr bool eclipsed(const AxBox4<T>& big, const AxBox4<T>& small)
    {
        return all_less_equal(big.lo, small.lo) && all_greater_equal(big.hi, small.hi);
    }

    template <typename T>
    auto            hypervolume(const AxBox4<T>& bx)
    {
        return component_product(bx.hi-bx.lo);
    }
    
    template <typename T>
    constexpr bool inside(const AxBox4<T>& bx, const Vec4<T>& pt)
    {
        return all_less_equal(bx.lo, pt) && all_less_equal(pt, bx.hi);
    }
    
    template <typename T>
    constexpr bool overlaps(const AxBox4<T>& a, const AxBox4<T>& b)
    {
        return all_less_equal(a.lo, b.hi) && all_greater_equal(a.hi, b.lo);
    }
}

YQ_TYPE_DECLARE(yq::AxBox4D)
YQ_TYPE_DECLARE(yq::AxBox4F)
YQ_TYPE_DECLARE(yq::AxBox4I)
YQ_TYPE_DECLARE(yq::AxBox4U)

