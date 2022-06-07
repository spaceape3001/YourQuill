////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math/vector_python.hpp>
#include <math/tensor_python.hpp>

namespace yq {

    template <typename T>
    square_t<T>     component_product(const Vec2<T>& a)
    {
        return a.x*a.y;
    }

    template <typename T>
    cube_t<T>       component_product(const Vec3<T>& a)
    {
        return a.x*a.y*a.z;
    }

    template <typename T>
    auto           component_product(const Vec4<T>& a)
    {
        return a.x*a.y*a.z*a.w;
    }

}
