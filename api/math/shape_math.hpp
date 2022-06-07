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
#include <math/shape/Seg1.hpp>
#include <math/shape/Seg2.hpp>
#include <math/shape/Seg3.hpp>
#include <math/shape/Seg4.hpp>
#include "vector_math.hpp"

namespace yq {


    template <typename T>
    AxBox1<T>   aabb(const Seg1<T>& a)
    {
        return aabb(a.lo, a.hi);
    }
    
    template <typename T>
    AxBox2<T>   aabb(const Seg2<T>& a)
    {
        return aabb(a.lo, a.hi);
    }
    
    template <typename T>
    AxBox3<T>   aabb(const Seg3<T>& a)
    {
        return aabb(a.lo, a.hi);
    }
    
    template <typename T>
    AxBox4<T>   aabb(const Seg4<T>& a)
    {
        return aabb(a.lo, a.hi);
    }
    
    


}

