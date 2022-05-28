////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

/*
    This file is for all the manually defined stuff for tensors
*/

#include <math/vector_python.hpp>
#include <math/tensor_python.hpp>

namespace yq {
    template <typename T>
    auto determinant(const Ten11<T>& a)
    {
        return a.xx;
    }

    template <typename T>
    auto determinant(const Ten22<T>& a)
    {
        return a.xx*a.yy-a.xy*a.yx;
    }

    template <typename T>
    auto determinant(const Ten33<T>& a)
    {
        return a.xx*(a.yy*a.zz-a.yz*a.zy)+a.xy*(a.zx*a.xz-a.yx*a.zz)+a.xz*(a.yx*a.zy-a.zx*a.yy);
    }
 
    //  4x4 determinant is TODO....
    
    template <typename T>
    Ten22<inverse_t<T>> inverse(const Ten22<T>&a)
    {
        auto    di = one_v<T> / determinant(a);
        return {
             di * a.yy, -di * a.xy,
            -di * a.yx,  di * a.xx
        };
    }
    
    template <typename T>
    Ten33<inverse_t<T>> inverse(const Ten33<T>&a)
    {
        auto    di = one_v<T> / determinant(a);
        return {
            di*(a.yy*a.zz-a.zy*a.yz), di*(a.xz*a.zy-a.zz*a.xy), di*(a.xy*a.yz-a.yy*a.xz),
            di*(a.yz*a.zx-a.zz*a.yx), di*(a.xx*a.zz-a.xz*a.zx), di*(a.xz*a.yx-a.yz*a.xx),
            di*(a.yx*a.zy-a.zx*a.yy), di*(a.xy*a.zx-a.zy*a.xx), di*(a.xx*a.yy-a.yx*a.xy)
        };
    }
}

