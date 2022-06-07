////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

//  WARNING!  WARNING!
//
//  THIS FILE IS AUTO-GENERATED, CHANGES WILL BE CLOBBERED!

#include <math/preamble.hpp>
#include <math/vec/Vec2.hpp>

namespace yq {

    /*! \brief Axially aligned box in 2 dimension(s).
    */
    template <typename T>
    struct AxBox2 {
        using component_t   = T;
        
        Vec2<T>  lo, hi;
        
        constexpr bool operator==(const AxBox2&) const noexcept = default;
    };
    
//  --------------------------------------------------------
//  COMPOSITION

    template <typename T>
    constexpr AxBox2<T> aabb(const Vec2<T>& a, const Vec2<T>& b)
    {
        return { min_elem(a,b), max_elem(a,b) };
    }
    
    YQ_NAN_1(AxBox2, { nan_v<Vec2>, nan_v<Vec2>});
    YQ_ZERO_1(AxBox2, { zero_v<Vec2>, zero_v<Vec2>});

//  --------------------------------------------------------
//  BASIC FUNCTIONS

    template <typename T>
    bool    valid(const AxBox2<T>& a)
    {
        return all_lees_equal(a.lo, a.hi);
    }

    YQ_IS_FINITE_1( AxBox2, is_finite(v.lo) && is_finite(v.hi))
    YQ_IS_NAN_1(AxBox2, is_nan(v.lo) || is_nan(v.hi))

//  --------------------------------------------------------
//  OPERATIONS

    /*! \brief Union of two AABBs
    */
    template <typename T>
    constexpr AxBox2<T> operator|(const AxBox2<T>&a, const AxBox2<T>&b)
    {
        return { min_elem(a.lo, b.lo), max_elem(a.hi, b.hi) };
    }

    /*! \brief Intersection of two AABBs
    */
    template <typename T>
    constexpr AxBox2<T> operator&(const AxBox2<T>&a, const AxBox2<T>&b)
    {
        return { max_elem(a.lo, b.lo), min_elem(a.hi, b.hi) };
    }

//  --------------------------------------------------------
//  ADVANCED FUNCTIONS
    
    template <typename T>
    square_t<T>     area(const AxBox2<T>& bx)
    {
        return component_product(bx.hi-bx.lo);
    }

    template <typename T>
    constexpr bool eclipsed(const AxBox2<T>& big, const AxBox2<T>& small)
    {
        return all_less_equal(big.lo, small.lo) && all_greater_equal(big.hi, small.hi);
    }
    
    template <typename T>
    constexpr bool inside(const AxBox2<T>& bx, const Vec2<T>& pt)
    {
        return all_less_equal(bx.lo, pt) && all_less_equal(pt, bx.hi);
    }
    
    template <typename T>
    constexpr bool overlaps(const AxBox2<T>& a, const AxBox2<T>& b)
    {
        return all_less_equal(a.lo, b.hi) && all_greater_equal(a.hi, b.lo);
    }
}

YQ_TYPE_DECLARE(yq::AxBox2D)
YQ_TYPE_DECLARE(yq::AxBox2F)
YQ_TYPE_DECLARE(yq::AxBox2I)
YQ_TYPE_DECLARE(yq::AxBox2U)

