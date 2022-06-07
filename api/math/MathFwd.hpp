////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cmath>
#include <stdint.h>
//#include <utility>
#include <yq/NaN.hpp>


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

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//      CONSTANTS
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


static constexpr double   pi      = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862;
static constexpr double   sqrt2   = 1.4142135623730950488016887242096980785696718753769480731766797379907324784621;
static constexpr double   sqrt3   = 1.7320508075688772935274463415058723669428052538103806280558069794519330169088;
static constexpr int64_t  max64   = std::numeric_limits<int64_t>::max();
static constexpr uint64_t maxu64  = std::numeric_limits<uint64_t>::max();

static constexpr double   deg2rad = pi / 180.0;
static constexpr double   rad2deg = 180.0 / pi;

template <typename T>
T           maximum( T a, T b) { return (a>b) ? a : b; }

template <typename T>
T           maximum( T a, T b, T c) { return maximum(maximum(a,b),c); }

template <typename T>
T           minimum( T a, T b) { return (a<b) ? a : b; }

template <typename T>
T           minimum( T a, T b, T c) { return minimum(minimum(a,b),c); }

template <typename T>
T   constrain(T value, T minV, T maxV)
{
    return maximum(minimum(value, maxV), minV);
}


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

constexpr bool  isNaN(double v)
{
    return std::isnan(v);
}

constexpr bool  isNaN(float v)
{
    return std::isnan(v);
}

//int gcd(int a, int b);
constexpr int   iround(double v)
{
    return (int)(std::round(v)+std::copysign(0.1,v));
}

constexpr int     iceil(double r)
{
    return (int)(ceil(r)+copysign(0.1,r));
}

constexpr int     itrunc(double r)
{
    return (int)(trunc(r)+copysign(0.1,r));
}

constexpr int     ifloor(double r)
{
    return (int)(floor(r)+copysign(0.1,r));
}


//  Forward declarations

struct AxBox2D;
struct AxBox2I;
struct AxBox3D;
struct AxBox3I;
struct AxBox4D;
struct Bi2D;
struct Bi3D;
struct Bi4D;
struct Coord2;
struct GeoD;
struct GeoM;
struct Mul1D;
struct Mul2D;
struct Mul3D;
struct Mul4D;
struct Polygon2I;
struct Quad4D;
struct Quat3D;
struct RangeD;
struct RangeI;
struct Segment2D;
struct Segment2I;
struct Tensor11D;
struct Tensor12D;
struct Tensor13D;
struct Tensor14D;
struct Tensor21D;
struct Tensor22D;
struct Tensor23D;
struct Tensor24D;
struct Tensor31D;
struct Tensor32D;
struct Tensor33D;
struct Tensor34D;
struct Tensor41D;
struct Tensor42D;
struct Tensor43D;
struct Tensor44D;
struct Tri3D;
struct Tri4D;
struct Vector1D;
struct Vector2D;
struct Vector2I;
struct Vector3D;
struct Vector3I;
struct Vector4D;

class Degree;
class Radian;

template <typename> class Range;

template <typename> struct Geo;
template <typename> struct Vector1;
template <typename> struct Vector2;
template <typename> struct Vector3;
template <typename> struct Vector4;

template <typename, typename> class KDTree2D;
template <typename, typename> class KDTree2I;
template <typename, typename> class KDTree3D;

#if 0
template <class,typename> class UValue;

template <class> class UMksFloat;
template <class> class UMksDouble;
template <class> class UMksVector2D;
template <class> class UMksVector3D;
template <class> class UMksVector4D;
template <class> class UMksTensor33D;

template <class B, class K> class UScaleFloat;
template <class B, class K> class UScaleDouble;
template <class B, class K> class UScaleVector2D;
template <class B, class K> class UScaleVector3D;
template <class B, class K> class UScaleVector4D;
template <class B, class K> class UScaleTensor33D;
#endif

#ifdef ENABLE_QT
    class QMatrix4x4;
    class QPoint;
    class QPointF;
    class QQuaternion;
    class QRectangle;
    class QRectangleF;

    class QVector2D;
    class QVector3D;
    class QVector4D;
#endif

//#include "math/unit/UnitsFwd.hpp"

