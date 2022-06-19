////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <math/preamble.hpp>
#include <math/vec/Vector1.hpp>

namespace yq {

    /*! \brief Axially aligned box in 1 dimension(s).
    */
    template <typename T>
    struct AxBox1 {
        using component_t   = T;
        
        Vector1<T>  lo, hi;
        
        constexpr bool operator==(const AxBox1&) const noexcept = default;
    };
    
//  --------------------------------------------------------
//  COMPOSITION

    template <typename T>
    constexpr AxBox1<T> aabb(const Vector1<T>& a, const Vector1<T>& b)
    {
        return { min_elem(a,b), max_elem(a,b) };
    }
    
    YQ_NAN_1(AxBox1, { nan_v<Vector1<T>>, nan_v<Vector1<T>>});
    YQ_ZERO_1(AxBox1, { zero_v<Vector1<T>>, zero_v<Vector1<T>>});

//  --------------------------------------------------------
//  BASIC FUNCTIONS

    template <typename T>
    bool    valid(const AxBox1<T>& a)
    {
        return all_lees_equal(a.lo, a.hi);
    }

    YQ_IS_FINITE_1( AxBox1, is_finite(v.lo) && is_finite(v.hi))
    YQ_IS_NAN_1(AxBox1, is_nan(v.lo) || is_nan(v.hi))

//  --------------------------------------------------------
//  OPERATIONS

    /*! \brief Union of two AABBs
    */
    template <typename T>
    constexpr AxBox1<T> operator|(const AxBox1<T>&a, const AxBox1<T>&b)
    {
        return { min_elem(a.lo, b.lo), max_elem(a.hi, b.hi) };
    }

    /*! \brief Intersection of two AABBs
    */
    template <typename T>
    constexpr AxBox1<T> operator&(const AxBox1<T>&a, const AxBox1<T>&b)
    {
        return { max_elem(a.lo, b.lo), min_elem(a.hi, b.hi) };
    }

//  --------------------------------------------------------
//  ADVANCED FUNCTIONS
    
    template <typename T>
    constexpr bool eclipsed(const AxBox1<T>& big, const AxBox1<T>& small)
    {
        return all_less_equal(big.lo, small.lo) && all_greater_equal(big.hi, small.hi);
    }
    
    template <typename T>
    constexpr bool inside(const AxBox1<T>& bx, const Vector1<T>& pt)
    {
        return all_less_equal(bx.lo, pt) && all_less_equal(pt, bx.hi);
    }
    
    template <typename T>
    constexpr bool overlaps(const AxBox1<T>& a, const AxBox1<T>& b)
    {
        return all_less_equal(a.lo, b.hi) && all_greater_equal(a.hi, b.lo);
    }
}

YQ_TYPE_DECLARE(yq::AxBox1D)
YQ_TYPE_DECLARE(yq::AxBox1F)
YQ_TYPE_DECLARE(yq::AxBox1I)
YQ_TYPE_DECLARE(yq::AxBox1U)

