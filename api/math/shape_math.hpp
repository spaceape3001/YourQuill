////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math/shape/AxBox1.hpp>
#include <math/shape/AxBox2.hpp>
#include <math/shape/AxBox3.hpp>
#include <math/shape/AxBox4.hpp>
#include <math/shape/Circle2.hpp>
#include <math/shape/Normal2.hpp>
#include <math/shape/Normal3.hpp>
#include <math/shape/Plane3.hpp>
#include <math/shape/Segment1.hpp>
#include <math/shape/Segment2.hpp>
#include <math/shape/Segment3.hpp>
#include <math/shape/Segment4.hpp>
#include <math/shape/Sphere3.hpp>
#include <math/shape/Sphere4.hpp>
#include "vector_math.hpp"
#include "tensor_math.hpp"

namespace yq {

//  --------------------------------------------------------
//  COMPOSITION

    template <typename T>
    AxBox2<T>   aabb(const Circle2<T>&a)
    {
        T       r   = abs(a.r);
        return {{
            a.pt.x - r,
            a.pt.y - r
        },{
            a.pt.x + r,
            a.pt.y + r
        }};
    }
    
    template <typename T>
    AxBox1<T>   aabb(const Segment1<T>& a)
    {
        return aabb(a.lo, a.hi);
    }
    
    template <typename T>
    AxBox2<T>   aabb(const Segment2<T>& a)
    {
        return aabb(a.lo, a.hi);
    }
    
    template <typename T>
    AxBox3<T>   aabb(const Segment3<T>& a)
    {
        return aabb(a.lo, a.hi);
    }
    
    template <typename T>
    AxBox4<T>   aabb(const Segment4<T>& a)
    {
        return aabb(a.lo, a.hi);
    }
    
    template <typename T>
    AxBox3<T>   aabb(const Sphere3<T>&a)
    {
        T       r   = abs(a.r);
        return {{
            a.pt.x - r,
            a.pt.y - r,
            a.pt.z - r
        },{
            a.pt.x + r,
            a.pt.y + r,
            a.pt.z + r
        }};
    }
    
    template <typename T>
    AxBox4<T>   aabb(const Sphere4<T>&a)
    {
        T       r   = abs(a.r);
        return {{
            a.pt.x - r,
            a.pt.y - r,
            a.pt.z - r,
            a.pt.w - r
        },{
            a.pt.x + r,
            a.pt.y + r,
            a.pt.z + r,
            a.pt.w + r
        }};
    }
    
//  --------------------------------------------------------
//  MULTIPLICATION

    template <typename T, typename U>
    Segment1<product_t<T,U>>    operator*(const Segment1<T>&a, const Tensor11<T>&b)
    {
        return { a.a * b, a.b * b };
    }

    template <typename T, typename U>
    Segment2<product_t<T,U>>    operator*(const Segment1<T>&a, const Tensor12<T>&b)
    {
        return { a.a * b, a.b * b };
    }

    template <typename T, typename U>
    Segment3<product_t<T,U>>    operator*(const Segment1<T>&a, const Tensor13<T>&b)
    {
        return { a.a * b, a.b * b };
    }

    template <typename T, typename U>
    Segment4<product_t<T,U>>    operator*(const Segment1<T>&a, const Tensor14<T>&b)
    {
        return { a.a * b, a.b * b };
    }

    template <typename T, typename U>
    Segment1<product_t<T,U>>    operator*(const Segment2<T>&a, const Tensor21<T>&b)
    {
        return { a.a * b, a.b * b };
    }

    template <typename T, typename U>
    Segment2<product_t<T,U>>    operator*(const Segment2<T>&a, const Tensor22<T>&b)
    {
        return { a.a * b, a.b * b };
    }

    template <typename T, typename U>
    Segment3<product_t<T,U>>    operator*(const Segment2<T>&a, const Tensor23<T>&b)
    {
        return { a.a * b, a.b * b };
    }

    template <typename T, typename U>
    Segment4<product_t<T,U>>    operator*(const Segment2<T>&a, const Tensor24<T>&b)
    {
        return { a.a * b, a.b * b };
    }

    template <typename T, typename U>
    Segment1<product_t<T,U>>    operator*(const Segment3<T>&a, const Tensor31<T>&b)
    {
        return { a.a * b, a.b * b };
    }

    template <typename T, typename U>
    Segment2<product_t<T,U>>    operator*(const Segment3<T>&a, const Tensor32<T>&b)
    {
        return { a.a * b, a.b * b };
    }

    template <typename T, typename U>
    Segment3<product_t<T,U>>    operator*(const Segment3<T>&a, const Tensor33<T>&b)
    {
        return { a.a * b, a.b * b };
    }

    template <typename T, typename U>
    Segment4<product_t<T,U>>    operator*(const Segment3<T>&a, const Tensor34<T>&b)
    {
        return { a.a * b, a.b * b };
    }

    template <typename T, typename U>
    Segment1<product_t<T,U>>    operator*(const Segment4<T>&a, const Tensor41<T>&b)
    {
        return { a.a * b, a.b * b };
    }

    template <typename T, typename U>
    Segment2<product_t<T,U>>    operator*(const Segment4<T>&a, const Tensor42<T>&b)
    {
        return { a.a * b, a.b * b };
    }

    template <typename T, typename U>
    Segment3<product_t<T,U>>    operator*(const Segment4<T>&a, const Tensor43<T>&b)
    {
        return { a.a * b, a.b * b };
    }

    template <typename T, typename U>
    Segment4<product_t<T,U>>    operator*(const Segment4<T>&a, const Tensor44<T>&b)
    {
        return { a.a * b, a.b * b };
    }

    //  ----------------------------------------------

    template <typename T, typename U>
    Triangle2<product_t<T,U>>    operator*(const Triangle2<T>&a, const Tensor22<T>&b)
    {
        return { a.a * b, a.b * b, a.c*b };
    }

    template <typename T, typename U>
    Triangle3<product_t<T,U>>    operator*(const Triangle2<T>&a, const Tensor23<T>&b)
    {
        return { a.a * b, a.b * b, a.c*b };
    }

    template <typename T, typename U>
    Triangle4<product_t<T,U>>    operator*(const Triangle2<T>&a, const Tensor24<T>&b)
    {
        return { a.a * b, a.b * b, a.c*b };
    }

    template <typename T, typename U>
    Triangle2<product_t<T,U>>    operator*(const Triangle3<T>&a, const Tensor32<T>&b)
    {
        return { a.a * b, a.b * b, a.c*b };
    }

    template <typename T, typename U>
    Triangle3<product_t<T,U>>    operator*(const Triangle3<T>&a, const Tensor33<T>&b)
    {
        return { a.a * b, a.b * b, a.c*b };
    }

    template <typename T, typename U>
    Triangle4<product_t<T,U>>    operator*(const Triangle3<T>&a, const Tensor34<T>&b)
    {
        return { a.a * b, a.b * b, a.c*b };
    }

    template <typename T, typename U>
    Triangle2<product_t<T,U>>    operator*(const Triangle4<T>&a, const Tensor42<T>&b)
    {
        return { a.a * b, a.b * b, a.c*b };
    }

    template <typename T, typename U>
    Triangle3<product_t<T,U>>    operator*(const Triangle4<T>&a, const Tensor43<T>&b)
    {
        return { a.a * b, a.b * b, a.c*b };
    }

    template <typename T, typename U>
    Triangle4<product_t<T,U>>    operator*(const Triangle4<T>&a, const Tensor44<T>&b)
    {
        return { a.a * b, a.b * b, a.c*b };
    }


    //  ----------------------------------------------

    template <typename T, typename U>
    Segment1<product_t<T,U>>    operator*(const Tensor11<T>&a, const Segment1<T>&b)
    {
        return { a * b.a, a * b.b };
    }
    
    template <typename T, typename U>
    Segment1<product_t<T,U>>    operator*(const Tensor12<T>&a, const Segment2<T>&b)
    {
        return { a * b.a, a * b.b };
    }

    template <typename T, typename U>
    Segment1<product_t<T,U>>    operator*(const Tensor13<T>&a, const Segment3<T>&b)
    {
        return { a * b.a, a * b.b };
    }

    template <typename T, typename U>
    Segment1<product_t<T,U>>    operator*(const Tensor14<T>&a, const Segment4<T>&b)
    {
        return { a * b.a, a * b.b };
    }

    template <typename T, typename U>
    Segment2<product_t<T,U>>    operator*(const Tensor21<T>&a, const Segment1<T>&b)
    {
        return { a * b.a, a * b.b };
    }
    
    template <typename T, typename U>
    Segment2<product_t<T,U>>    operator*(const Tensor22<T>&a, const Segment2<T>&b)
    {
        return { a * b.a, a * b.b };
    }

    template <typename T, typename U>
    Segment2<product_t<T,U>>    operator*(const Tensor23<T>&a, const Segment3<T>&b)
    {
        return { a * b.a, a * b.b };
    }

    template <typename T, typename U>
    Segment2<product_t<T,U>>    operator*(const Tensor24<T>&a, const Segment4<T>&b)
    {
        return { a * b.a, a * b.b };
    }

    template <typename T, typename U>
    Segment3<product_t<T,U>>    operator*(const Tensor31<T>&a, const Segment1<T>&b)
    {
        return { a * b.a, a * b.b };
    }
    
    template <typename T, typename U>
    Segment3<product_t<T,U>>    operator*(const Tensor32<T>&a, const Segment2<T>&b)
    {
        return { a * b.a, a * b.b };
    }

    template <typename T, typename U>
    Segment3<product_t<T,U>>    operator*(const Tensor33<T>&a, const Segment3<T>&b)
    {
        return { a * b.a, a * b.b };
    }

    template <typename T, typename U>
    Segment3<product_t<T,U>>    operator*(const Tensor34<T>&a, const Segment4<T>&b)
    {
        return { a * b.a, a * b.b };
    }

    template <typename T, typename U>
    Segment4<product_t<T,U>>    operator*(const Tensor41<T>&a, const Segment1<T>&b)
    {
        return { a * b.a, a * b.b };
    }
    
    template <typename T, typename U>
    Segment4<product_t<T,U>>    operator*(const Tensor42<T>&a, const Segment2<T>&b)
    {
        return { a * b.a, a * b.b };
    }

    template <typename T, typename U>
    Segment4<product_t<T,U>>    operator*(const Tensor43<T>&a, const Segment3<T>&b)
    {
        return { a * b.a, a * b.b };
    }

    template <typename T, typename U>
    Segment4<product_t<T,U>>    operator*(const Tensor44<T>&a, const Segment4<T>&b)
    {
        return { a * b.a, a * b.b };
    }

    //  ----------------------------------------------

    
    template <typename T, typename U>
    Triangle2<product_t<T,U>>    operator*(const Tensor22<T>&a, const Triangle2<T>&b)
    {
        return { a * b.a, a * b.b, a * b.c };
    }

    template <typename T, typename U>
    Triangle2<product_t<T,U>>    operator*(const Tensor23<T>&a, const Triangle3<T>&b)
    {
        return { a * b.a, a * b.b, a * b.c };
    }

    template <typename T, typename U>
    Triangle2<product_t<T,U>>    operator*(const Tensor24<T>&a, const Triangle4<T>&b)
    {
        return { a * b.a, a * b.b, a * b.c };
    }

    template <typename T, typename U>
    Triangle3<product_t<T,U>>    operator*(const Tensor32<T>&a, const Triangle2<T>&b)
    {
        return { a * b.a, a * b.b, a * b.c };
    }

    template <typename T, typename U>
    Triangle3<product_t<T,U>>    operator*(const Tensor33<T>&a, const Triangle3<T>&b)
    {
        return { a * b.a, a * b.b, a * b.c };
    }

    template <typename T, typename U>
    Triangle3<product_t<T,U>>    operator*(const Tensor34<T>&a, const Triangle4<T>&b)
    {
        return { a * b.a, a * b.b, a * b.c };
    }

    template <typename T, typename U>
    Triangle4<product_t<T,U>>    operator*(const Tensor42<T>&a, const Triangle2<T>&b)
    {
        return { a * b.a, a * b.b, a * b.c };
    }

    template <typename T, typename U>
    Triangle4<product_t<T,U>>    operator*(const Tensor43<T>&a, const Triangle3<T>&b)
    {
        return { a * b.a, a * b.b, a * b.c };
    }

    template <typename T, typename U>
    Triangle4<product_t<T,U>>    operator*(const Tensor44<T>&a, const Triangle4<T>&b)
    {
        return { a * b.a, a * b.b, a * b.c };
    }

//  --------------------------------------------------------
//  ADVANCED FUNCTIONS


}

