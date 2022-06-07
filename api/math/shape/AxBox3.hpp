////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <math/preamble.hpp>
#include <math/vec/Vec3.hpp>

namespace yq {

    /*! \brief Axially aligned box in 3 dimension(s).
    */
    template <typename T>
    struct AxBox3 {
        using component_t   = T;
        
        Vec3<T>  lo, hi;
        
        constexpr bool operator==(const AxBox3&) const noexcept = default;
    };
    
//  --------------------------------------------------------
//  COMPOSITION

    template <typename T>
    constexpr AxBox3<T> aabb(const Vec3<T>& a, const Vec3<T>& b)
    {
        return { min_elem(a,b), max_elem(a,b) };
    }
    
    YQ_NAN_1(AxBox3, { nan_v<Vec3>, nan_v<Vec3>});
    YQ_ZERO_1(AxBox3, { zero_v<Vec3>, zero_v<Vec3>});

//  --------------------------------------------------------
//  BASIC FUNCTIONS

    template <typename T>
    bool    valid(const AxBox3<T>& a)
    {
        return all_lees_equal(a.lo, a.hi);
    }

    YQ_IS_FINITE_1( AxBox3, is_finite(v.lo) && is_finite(v.hi))
    YQ_IS_NAN_1(AxBox3, is_nan(v.lo) || is_nan(v.hi))

//  --------------------------------------------------------
//  OPERATIONS

    /*! \brief Union of two AABBs
    */
    template <typename T>
    constexpr AxBox3<T> operator|(const AxBox3<T>&a, const AxBox3<T>&b)
    {
        return { min_elem(a.lo, b.lo), max_elem(a.hi, b.hi) };
    }

    /*! \brief Intersection of two AABBs
    */
    template <typename T>
    constexpr AxBox3<T> operator&(const AxBox3<T>&a, const AxBox3<T>&b)
    {
        return { max_elem(a.lo, b.lo), min_elem(a.hi, b.hi) };
    }

//  --------------------------------------------------------
//  ADVANCED FUNCTIONS
    
    template <typename T>
    constexpr bool eclipsed(const AxBox3<T>& big, const AxBox3<T>& small)
    {
        return all_less_equal(big.lo, small.lo) && all_greater_equal(big.hi, small.hi);
    }
    
    template <typename T>
    constexpr bool inside(const AxBox3<T>& bx, const Vec3<T>& pt)
    {
        return all_less_equal(bx.lo, pt) && all_less_equal(pt, bx.hi);
    }
    
    template <typename T>
    constexpr bool overlaps(const AxBox3<T>& a, const AxBox3<T>& b)
    {
        return all_less_equal(a.lo, b.hi) && all_greater_equal(a.hi, b.lo);
    }

    template <typename T>
    cube_t<T>       volume(const AxBox3<T>& bx)
    {
        return component_product(bx.hi-bx.lo);
    }

}

YQ_TYPE_DECLARE(yq::AxBox3D)
YQ_TYPE_DECLARE(yq::AxBox3F)
YQ_TYPE_DECLARE(yq::AxBox3I)
YQ_TYPE_DECLARE(yq::AxBox3U)

