////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/preamble.hpp>
#include <basic/trait/always_false.hpp>

#include <math/trait/has_abs.hpp>
#include <math/trait/has_is_finite.hpp>
#include <math/trait/has_nan.hpp>
#include <math/trait/has_one.hpp>
#include <math/trait/has_sqrt.hpp>
#include <math/trait/has_zero.hpp>
#include <cmath>
#include <cstdint>
#include <limits>
#include <numbers>
#include <utility>


    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //      MACROS
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#ifdef PI
#undef PI
#endif

/*! \brief Dot product operator

    As C++ doesn't let us define new operators, we must make do with the
    existing operators.  
    
    This is the macro used for the dot product between zvecs.
    
    \note   It is HIGHLY recommended to use parathensis as the operational 
        priorities are based on C++'s interpretation of the actual symbol,
        which is not necessarily our intended order.
*/
#define DOT         |

/*! \brief Inner product operator

    As C++ doesn't let us define new operators, we must make do with the
    existing operators.  
    
    This is the macro used for the inner product.

    \note   It is HIGHLY recommended to use parathensis as the operational 
        priorities are based on C++'s interpretation of the actual symbol,
        which is not necessarily our intended order.
*/
#define INNER       &
#define INNER_EQ    &=

/*! \brief Outer product operator

    As C++ doesn't let us define new operators, we must make do with the
    existing operators.  
    
    This is the macro used for the outer product.

    \note   It is HIGHLY recommended to use parathensis as the operational 
        priorities are based on C++'s interpretation of the actual symbol,
        which is not necessarily our intended order.
*/
#define OUTER       ^
#define OUTER_EQ    ^=

/*! \brief Outer Vector to Tensor operator

    As C++ doesn't let us define new operators, we must make do with the
    existing operators.  
    
    This is the macro used for the outer zvec product between zvecs.
    This is different than the general outer product because this one
    produces ztens, whereas the general outer product makes for zbvecs.
    
    Also, this operator can produce non-square ztens (ie, 3x2).

    \note   It is HIGHLY recommended to use parathensis as the operational 
        priorities are based on C++'s interpretation of the actual symbol,
        which is not necessarily our intended order.
*/
#define OTIMES      &&

/*! \brief Cross product operator

    As C++ doesn't let us define new operators, we must make do with the
    existing operators.  
    
    This is the macro used for the cross product between zvecs.

    \note   It is HIGHLY recommended to use parathensis as the operational 
        priorities are based on C++'s interpretation of the actual symbol,
        which is not necessarily our intended order.
*/
#define CROSS       %
#define CROSS_EQ   %=

namespace yq {
    static constexpr const double   pi      = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862;
    static constexpr const double   sqrt2   = 1.4142135623730950488016887242096980785696718753769480731766797379907324784621;
    static constexpr const double   sqrt3   = 1.7320508075688772935274463415058723669428052538103806280558069794519330169088;
    static constexpr const int64_t  max64   = std::numeric_limits<int64_t>::max();
    static constexpr const uint64_t maxu64  = std::numeric_limits<uint64_t>::max();

    static constexpr double   deg2rad = pi / 180.0;
    static constexpr double   rad2deg = 180.0 / pi;

    template <typename F, typename S>
    S       greatest(const std::pair<F,S>& a, const std::pair<F,S>& b)
    {
        if(a.first >= b.first)
            return a.second;
        else
            return b.second;
    }

    template <typename F, typename S>
    S       greatest(const std::pair<F,S>& a, const std::pair<F,S>& b, const std::pair<F,S>& c)
    {
        if(c.first >= std::max(a.first, b.first))
            return c.second;
        else if(b.first >= std::max(a.first, c.first))
            return b.second;
        else
            return a.second;
    }


    inline constexpr int   iround(double v)
    {
        return (int)(std::round(v)+std::copysign(0.1,v));
    }

    inline constexpr int   iceil(double r)
    {
        return (int)(ceil(r)+copysign(0.1,r));
    }

    inline constexpr int   itrunc(double r)
    {
        return (int)(trunc(r)+copysign(0.1,r));
    }

    inline constexpr int   ifloor(double r)
    {
        return (int)(floor(r)+copysign(0.1,r));
    }


    template <typename T> struct Coord2;
    using Coord2D = Coord2<double>;
    using Coord2F = Coord2<float>;
    using Coord2I = Coord2<int>;
    using Coord2U = Coord2<unsigned>;

    template <typename T> struct Fraction;
    using Frac8  = Fraction<int8_t>;
    using Frac16 = Fraction<int16_t>;
    using Frac32 = Fraction<int32_t>;
    using Frac64 = Fraction<int64_t>;
    
    template <typename T> struct Size2;
    using Size2D    = Size2<double>;
    using Size2F    = Size2<float>;
    using Size2I    = Size2<int>;
    using Size2I8   = Size2<int8_t>;
    using Size2I16  = Size2<int16_t>;
    using Size2I32  = Size2<int32_t>;
    using Size2I64  = Size2<int64_t>;
    using Size2U    = Size2<unsigned>;
    using Size2U8   = Size2<int8_t>;
    using Size2U16  = Size2<int16_t>;
    using Size2U32  = Size2<int32_t>;
    using Size2U64  = Size2<int64_t>;
    
    template <typename T> struct Vec1;
    using Vec1D     = Vec1<double>;
    using Vec1F     = Vec1<float>;
    using Vec1I     = Vec1<int>;
    using Vec1U     = Vec1<unsigned>;

    template <typename T> struct Vec2;
    using Vec2D     = Vec2<double>;
    using Vec2F     = Vec2<float>;
    using Vec2I     = Vec2<int>;
    using Vec2U     = Vec2<unsigned>;

    template <typename T> struct Vec3;
    using Vec3D     = Vec3<double>;
    using Vec3F     = Vec3<float>;
    using Vec3I     = Vec3<int>;
    using Vec3U     = Vec3<unsigned>;

    template <typename T> struct Vec4;
    using Vec4D     = Vec4<double>;
    using Vec4F     = Vec4<float>;
    using Vec4I     = Vec4<int>;
    using Vec4U     = Vec4<unsigned>;
    
    //! Call this if math isn't getting startup-initialized 
    void        initialize_math();

}



