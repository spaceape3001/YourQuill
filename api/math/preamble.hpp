////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/preamble.hpp>

#include <basic/StreamOps.hpp>
#include <basic/meta/InfoBinder.hpp>
#include <basic/trait/always_false.hpp>

#include <math/trait/has_abs.hpp>
#include <math/trait/has_copysign.hpp>
#include <math/trait/has_identity.hpp>
#include <math/trait/has_is_finite.hpp>
#include <math/trait/has_nan.hpp>
#include <math/trait/has_one.hpp>
#include <math/trait/has_sqrt.hpp>
#include <math/trait/has_zero.hpp>
#include <math/trait/ieee754.hpp>

#include <math/forward.hpp>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <numbers>
#include <utility>

#include <log4cpp/CategoryStream.hh>

#include <glm/glm.hpp>

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

namespace std {
    template <typename> class complex;
}


namespace yq {
    using ComplexD  = std::complex<double>;
    using ComplexF  = std::complex<float>;
    using ComplexI  = std::complex<int>;
    using ComplexU  = std::complex<unsigned>;
    

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


    inline int   iround(double v)
    {
        return (int)(std::round(v)+std::copysign(0.1,v));
    }

    inline int   iceil(double r)
    {
        return (int)(ceil(r)+std::copysign(0.1,r));
    }

    inline int   itrunc(double r)
    {
        return (int)(trunc(r)+std::copysign(0.1,r));
    }

    inline int   ifloor(double r)
    {
        return (int)(floor(r)+std::copysign(0.1,r));
    }

    template <typename T> struct AxBox1;
    using AxBox1D   = AxBox1<double>;
    using AxBox1F   = AxBox1<float>;
    using AxBox1I   = AxBox1<int>;
    using AxBox1U   = AxBox1<unsigned>;

    template <typename T> struct AxBox2;
    using AxBox2D   = AxBox2<double>;
    using AxBox2F   = AxBox2<float>;
    using AxBox2I   = AxBox2<int>;
    using AxBox2U   = AxBox2<unsigned>;

    template <typename T> struct AxBox3;
    using AxBox3D   = AxBox3<double>;
    using AxBox3F   = AxBox3<float>;
    using AxBox3I   = AxBox3<int>;
    using AxBox3U   = AxBox3<unsigned>;

    template <typename T> struct AxBox4;
    using AxBox4D   = AxBox4<double>;
    using AxBox4F   = AxBox4<float>;
    using AxBox4I   = AxBox4<int>;
    using AxBox4U   = AxBox4<unsigned>;

    template <typename T> struct Circle2;
    using Circle2D   = Circle2<double>;
    using Circle2F   = Circle2<float>;
    using Circle2I   = Circle2<int>;
    using Circle2U   = Circle2<unsigned>;

    template <typename T> struct ColorRgb;
    using ColorRgbD     = ColorRgb<double>;
    using ColorRgbF     = ColorRgb<float>;
    using ColorRgbI     = ColorRgb<int>;
    using ColorRgbU     = ColorRgb<unsigned>;
    using ColorRgbU8    = ColorRgb<uint8_t>;
    using ColorRgbU16   = ColorRgb<uint16_t>;

    template <typename T> struct ColorRgba;
    using ColorRgbaD    = ColorRgba<double>;
    using ColorRgbaF    = ColorRgba<float>;
    using ColorRgbaU8   = ColorRgba<uint8_t>;
    using ColorRgbaU16  = ColorRgba<uint16_t>;
    

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
    
    template <typename T> struct Normal2;
    using Normal2D  = Normal2<double>;
    using Normal2F  = Normal2<float>;
    
    template <typename T> struct Normal3;
    using Normal3D  = Normal3<double>;
    using Normal3F  = Normal3<float>;


    template <typename T> struct Plane3;
    using Plane3D       = Plane3<double>;
    using Plane3F       = Plane3<float>;

    template <typename T> struct Quaternion3;
    using Quaternion3D  = Quaternion3<double>;
    using Quaternion3F  = Quaternion3<float>;
    
    template <typename T> struct Rectangle2;
    using Rectangle2D    = Rectangle2<double>;
    using Rectangle2F    = Rectangle2<float>;
    using Rectangle2I    = Rectangle2<int>;
    using Rectangle2U    = Rectangle2<unsigned>;

    template <typename T> struct Segment1;
    using Segment1D   = Segment1<double>;
    using Segment1F   = Segment1<float>;
    using Segment1I   = Segment1<int>;
    using Segment1U   = Segment1<unsigned>;

    template <typename T> struct Segment2;
    using Segment2D   = Segment2<double>;
    using Segment2F   = Segment2<float>;
    using Segment2I   = Segment2<int>;
    using Segment2U   = Segment2<unsigned>;

    template <typename T> struct Segment3;
    using Segment3D   = Segment3<double>;
    using Segment3F   = Segment3<float>;
    using Segment3I   = Segment3<int>;
    using Segment3U   = Segment3<unsigned>;

    template <typename T> struct Segment4;
    using Segment4D   = Segment4<double>;
    using Segment4F   = Segment4<float>;
    using Segment4I   = Segment4<int>;
    using Segment4U   = Segment4<unsigned>;
    
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
    using Size2Z    = Size2<size_t>;

    template <typename T> struct Sphere3;
    using Sphere3D   = Sphere3<double>;
    using Sphere3F   = Sphere3<float>;
    using Sphere3I   = Sphere3<int>;
    using Sphere3U   = Sphere3<unsigned>;
    
    template <typename T> struct Sphere4;
    using Sphere4D   = Sphere4<double>;
    using Sphere4F   = Sphere4<float>;
    using Sphere4I   = Sphere4<int>;
    using Sphere4U   = Sphere4<unsigned>;
    
    template <typename T> struct Tetrahedron3;
    using Tetrahedron3D   = Tetrahedron3<double>;
    using Tetrahedron3F   = Tetrahedron3<float>;
    using Tetrahedron3I   = Tetrahedron3<int>;
    using Tetrahedron3U   = Tetrahedron3<unsigned>;

    template <typename T> struct Triangle2;
    using Triangle2D   = Triangle2<double>;
    using Triangle2F   = Triangle2<float>;
    using Triangle2I   = Triangle2<int>;
    using Triangle2U   = Triangle2<unsigned>;

    template <typename T> struct Triangle3;
    using Triangle3D   = Triangle3<double>;
    using Triangle3F   = Triangle3<float>;
    using Triangle3I   = Triangle3<int>;
    using Triangle3U   = Triangle3<unsigned>;

    template <typename T> struct Triangle4;
    using Triangle4D   = Triangle4<double>;
    using Triangle4F   = Triangle4<float>;
    using Triangle4I   = Triangle4<int>;
    using Triangle4U   = Triangle4<unsigned>;

    
    //! Call this if math isn't getting startup-initialized 
    void        initialize_math();

    template <typename T> using square_t = decltype(T()*T());
    template <typename T> using cube_t   = decltype(T()*T()*T());
    template <typename T, typename U>  using product_t     = decltype(T()*U());
    template <typename T, typename U>  using quotient_t    = decltype(T()/U());
    template <typename T> using inverse_t = decltype(T()/square_t<T>());
    
    using std::max;
    using std::min;
    using std::abs;
    
    namespace trait {
        template <typename T, typename U>  static constexpr const bool self_mul_v = std::is_same_v<T,product_t<T,U>>;
        template <typename T, typename U>  static constexpr const bool self_div_v = std::is_same_v<T,quotient_t<T,U>>;
    }
}

