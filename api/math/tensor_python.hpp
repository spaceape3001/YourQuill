////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

//  WARNING!  WARNING!
//
//  THIS FILE IS AUTO-GENERATED, CHANGES WILL BE CLOBBERED!

#include <math/vec/Vec1.hpp>
#include <math/vec/Vec2.hpp>
#include <math/vec/Vec3.hpp>
#include <math/vec/Vec4.hpp>
#include <math/vec/Ten11.hpp>
#include <math/vec/Ten12.hpp>
#include <math/vec/Ten13.hpp>
#include <math/vec/Ten14.hpp>
#include <math/vec/Ten21.hpp>
#include <math/vec/Ten22.hpp>
#include <math/vec/Ten23.hpp>
#include <math/vec/Ten24.hpp>
#include <math/vec/Ten31.hpp>
#include <math/vec/Ten32.hpp>
#include <math/vec/Ten33.hpp>
#include <math/vec/Ten34.hpp>
#include <math/vec/Ten41.hpp>
#include <math/vec/Ten42.hpp>
#include <math/vec/Ten43.hpp>
#include <math/vec/Ten44.hpp>


namespace yq {

    template <typename T, typename U>
    constexpr Ten12<product_t<T,U>> operator*(const Ten11<T>& a, const Ten12<U>& b)
    {
        return {
            a.xx*b.xx,
            a.xx*b.xy
        };
    }
    
    template <typename T, typename U>
    constexpr Ten13<product_t<T,U>> operator*(const Ten11<T>& a, const Ten13<U>& b)
    {
        return {
            a.xx*b.xx,
            a.xx*b.xy,
            a.xx*b.xz
        };
    }
    
    template <typename T, typename U>
    constexpr Ten14<product_t<T,U>> operator*(const Ten11<T>& a, const Ten14<U>& b)
    {
        return {
            a.xx*b.xx,
            a.xx*b.xy,
            a.xx*b.xz,
            a.xx*b.xw
        };
    }
    
    template <typename T>
    constexpr Ten21<T>  transpose(const Ten12<T>&v)
    {
        return {
            v.xx,
            v.xy
        };
    }

    template <typename T, typename U>
    constexpr Ten11<product_t<T,U>> operator*(const Ten12<T>& a, const Ten21<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx
        };
    }
    
    template <typename T, typename U>
    constexpr Ten12<product_t<T,U>> operator*(const Ten12<T>& a, const Ten22<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx,
            a.xx*b.xy + a.xy*b.yy
        };
    }
    
    template <typename T, typename U>
    requires trait::self_mul_v<T,U>
    Ten12<T>& operator*=(Ten12<T>&a, const Ten22<U>& b)
    {
        a = a * b;
        return a;
    }

    template <typename T, typename U>
    constexpr Ten13<product_t<T,U>> operator*(const Ten12<T>& a, const Ten23<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx,
            a.xx*b.xy + a.xy*b.yy,
            a.xx*b.xz + a.xy*b.yz
        };
    }
    
    template <typename T, typename U>
    constexpr Ten14<product_t<T,U>> operator*(const Ten12<T>& a, const Ten24<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx,
            a.xx*b.xy + a.xy*b.yy,
            a.xx*b.xz + a.xy*b.yz,
            a.xx*b.xw + a.xy*b.yw
        };
    }
    
    template <typename T>
    constexpr Ten31<T>  transpose(const Ten13<T>&v)
    {
        return {
            v.xx,
            v.xy,
            v.xz
        };
    }

    template <typename T, typename U>
    constexpr Ten11<product_t<T,U>> operator*(const Ten13<T>& a, const Ten31<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx
        };
    }
    
    template <typename T, typename U>
    constexpr Ten12<product_t<T,U>> operator*(const Ten13<T>& a, const Ten32<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx,
            a.xx*b.xy + a.xy*b.yy + a.xz*b.zy
        };
    }
    
    template <typename T, typename U>
    constexpr Ten13<product_t<T,U>> operator*(const Ten13<T>& a, const Ten33<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx,
            a.xx*b.xy + a.xy*b.yy + a.xz*b.zy,
            a.xx*b.xz + a.xy*b.yz + a.xz*b.zz
        };
    }
    
    template <typename T, typename U>
    requires trait::self_mul_v<T,U>
    Ten13<T>& operator*=(Ten13<T>&a, const Ten33<U>& b)
    {
        a = a * b;
        return a;
    }

    template <typename T, typename U>
    constexpr Ten14<product_t<T,U>> operator*(const Ten13<T>& a, const Ten34<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx,
            a.xx*b.xy + a.xy*b.yy + a.xz*b.zy,
            a.xx*b.xz + a.xy*b.yz + a.xz*b.zz,
            a.xx*b.xw + a.xy*b.yw + a.xz*b.zw
        };
    }
    
    template <typename T>
    constexpr Ten41<T>  transpose(const Ten14<T>&v)
    {
        return {
            v.xx,
            v.xy,
            v.xz,
            v.xw
        };
    }

    template <typename T, typename U>
    constexpr Ten11<product_t<T,U>> operator*(const Ten14<T>& a, const Ten41<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx + a.xw*b.wx
        };
    }
    
    template <typename T, typename U>
    constexpr Ten12<product_t<T,U>> operator*(const Ten14<T>& a, const Ten42<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx + a.xw*b.wx,
            a.xx*b.xy + a.xy*b.yy + a.xz*b.zy + a.xw*b.wy
        };
    }
    
    template <typename T, typename U>
    constexpr Ten13<product_t<T,U>> operator*(const Ten14<T>& a, const Ten43<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx + a.xw*b.wx,
            a.xx*b.xy + a.xy*b.yy + a.xz*b.zy + a.xw*b.wy,
            a.xx*b.xz + a.xy*b.yz + a.xz*b.zz + a.xw*b.wz
        };
    }
    
    template <typename T, typename U>
    constexpr Ten14<product_t<T,U>> operator*(const Ten14<T>& a, const Ten44<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx + a.xw*b.wx,
            a.xx*b.xy + a.xy*b.yy + a.xz*b.zy + a.xw*b.wy,
            a.xx*b.xz + a.xy*b.yz + a.xz*b.zz + a.xw*b.wz,
            a.xx*b.xw + a.xy*b.yw + a.xz*b.zw + a.xw*b.ww
        };
    }
    
    template <typename T, typename U>
    requires trait::self_mul_v<T,U>
    Ten14<T>& operator*=(Ten14<T>&a, const Ten44<U>& b)
    {
        a = a * b;
        return a;
    }

    template <typename T>
    constexpr Ten12<T>  transpose(const Ten21<T>&v)
    {
        return {
            v.xx, v.yx
        };
    }

    template <typename T, typename U>
    constexpr Ten21<product_t<T,U>> operator*(const Ten21<T>& a, const Ten11<U>& b)
    {
        return {
            a.xx*b.xx,

            a.yx*b.xx
        };
    }
    
    template <typename T, typename U>
    requires trait::self_mul_v<T,U>
    Ten21<T>& operator*=(Ten21<T>&a, const Ten11<U>& b)
    {
        a = a * b;
        return a;
    }

    template <typename T, typename U>
    constexpr Ten22<product_t<T,U>> operator*(const Ten21<T>& a, const Ten12<U>& b)
    {
        return {
            a.xx*b.xx,
            a.xx*b.xy,

            a.yx*b.xx,
            a.yx*b.xy
        };
    }
    
    template <typename T, typename U>
    constexpr Ten23<product_t<T,U>> operator*(const Ten21<T>& a, const Ten13<U>& b)
    {
        return {
            a.xx*b.xx,
            a.xx*b.xy,
            a.xx*b.xz,

            a.yx*b.xx,
            a.yx*b.xy,
            a.yx*b.xz
        };
    }
    
    template <typename T, typename U>
    constexpr Ten24<product_t<T,U>> operator*(const Ten21<T>& a, const Ten14<U>& b)
    {
        return {
            a.xx*b.xx,
            a.xx*b.xy,
            a.xx*b.xz,
            a.xx*b.xw,

            a.yx*b.xx,
            a.yx*b.xy,
            a.yx*b.xz,
            a.yx*b.xw
        };
    }
    
    template <typename T, typename U>
    constexpr Ten21<product_t<T,U>> operator*(const Ten22<T>& a, const Ten21<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx,

            a.yx*b.xx + a.yy*b.yx
        };
    }
    
    template <typename T, typename U>
    constexpr Ten23<product_t<T,U>> operator*(const Ten22<T>& a, const Ten23<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx,
            a.xx*b.xy + a.xy*b.yy,
            a.xx*b.xz + a.xy*b.yz,

            a.yx*b.xx + a.yy*b.yx,
            a.yx*b.xy + a.yy*b.yy,
            a.yx*b.xz + a.yy*b.yz
        };
    }
    
    template <typename T, typename U>
    constexpr Ten24<product_t<T,U>> operator*(const Ten22<T>& a, const Ten24<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx,
            a.xx*b.xy + a.xy*b.yy,
            a.xx*b.xz + a.xy*b.yz,
            a.xx*b.xw + a.xy*b.yw,

            a.yx*b.xx + a.yy*b.yx,
            a.yx*b.xy + a.yy*b.yy,
            a.yx*b.xz + a.yy*b.yz,
            a.yx*b.xw + a.yy*b.yw
        };
    }
    
    template <typename T>
    constexpr Ten32<T>  transpose(const Ten23<T>&v)
    {
        return {
            v.xx, v.yx,
            v.xy, v.yy,
            v.xz, v.yz
        };
    }

    template <typename T, typename U>
    constexpr Ten21<product_t<T,U>> operator*(const Ten23<T>& a, const Ten31<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx,

            a.yx*b.xx + a.yy*b.yx + a.yz*b.zx
        };
    }
    
    template <typename T, typename U>
    constexpr Ten22<product_t<T,U>> operator*(const Ten23<T>& a, const Ten32<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx,
            a.xx*b.xy + a.xy*b.yy + a.xz*b.zy,

            a.yx*b.xx + a.yy*b.yx + a.yz*b.zx,
            a.yx*b.xy + a.yy*b.yy + a.yz*b.zy
        };
    }
    
    template <typename T, typename U>
    constexpr Ten23<product_t<T,U>> operator*(const Ten23<T>& a, const Ten33<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx,
            a.xx*b.xy + a.xy*b.yy + a.xz*b.zy,
            a.xx*b.xz + a.xy*b.yz + a.xz*b.zz,

            a.yx*b.xx + a.yy*b.yx + a.yz*b.zx,
            a.yx*b.xy + a.yy*b.yy + a.yz*b.zy,
            a.yx*b.xz + a.yy*b.yz + a.yz*b.zz
        };
    }
    
    template <typename T, typename U>
    requires trait::self_mul_v<T,U>
    Ten23<T>& operator*=(Ten23<T>&a, const Ten33<U>& b)
    {
        a = a * b;
        return a;
    }

    template <typename T, typename U>
    constexpr Ten24<product_t<T,U>> operator*(const Ten23<T>& a, const Ten34<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx,
            a.xx*b.xy + a.xy*b.yy + a.xz*b.zy,
            a.xx*b.xz + a.xy*b.yz + a.xz*b.zz,
            a.xx*b.xw + a.xy*b.yw + a.xz*b.zw,

            a.yx*b.xx + a.yy*b.yx + a.yz*b.zx,
            a.yx*b.xy + a.yy*b.yy + a.yz*b.zy,
            a.yx*b.xz + a.yy*b.yz + a.yz*b.zz,
            a.yx*b.xw + a.yy*b.yw + a.yz*b.zw
        };
    }
    
    template <typename T>
    constexpr Ten42<T>  transpose(const Ten24<T>&v)
    {
        return {
            v.xx, v.yx,
            v.xy, v.yy,
            v.xz, v.yz,
            v.xw, v.yw
        };
    }

    template <typename T, typename U>
    constexpr Ten21<product_t<T,U>> operator*(const Ten24<T>& a, const Ten41<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx + a.xw*b.wx,

            a.yx*b.xx + a.yy*b.yx + a.yz*b.zx + a.yw*b.wx
        };
    }
    
    template <typename T, typename U>
    constexpr Ten22<product_t<T,U>> operator*(const Ten24<T>& a, const Ten42<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx + a.xw*b.wx,
            a.xx*b.xy + a.xy*b.yy + a.xz*b.zy + a.xw*b.wy,

            a.yx*b.xx + a.yy*b.yx + a.yz*b.zx + a.yw*b.wx,
            a.yx*b.xy + a.yy*b.yy + a.yz*b.zy + a.yw*b.wy
        };
    }
    
    template <typename T, typename U>
    constexpr Ten23<product_t<T,U>> operator*(const Ten24<T>& a, const Ten43<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx + a.xw*b.wx,
            a.xx*b.xy + a.xy*b.yy + a.xz*b.zy + a.xw*b.wy,
            a.xx*b.xz + a.xy*b.yz + a.xz*b.zz + a.xw*b.wz,

            a.yx*b.xx + a.yy*b.yx + a.yz*b.zx + a.yw*b.wx,
            a.yx*b.xy + a.yy*b.yy + a.yz*b.zy + a.yw*b.wy,
            a.yx*b.xz + a.yy*b.yz + a.yz*b.zz + a.yw*b.wz
        };
    }
    
    template <typename T, typename U>
    constexpr Ten24<product_t<T,U>> operator*(const Ten24<T>& a, const Ten44<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx + a.xw*b.wx,
            a.xx*b.xy + a.xy*b.yy + a.xz*b.zy + a.xw*b.wy,
            a.xx*b.xz + a.xy*b.yz + a.xz*b.zz + a.xw*b.wz,
            a.xx*b.xw + a.xy*b.yw + a.xz*b.zw + a.xw*b.ww,

            a.yx*b.xx + a.yy*b.yx + a.yz*b.zx + a.yw*b.wx,
            a.yx*b.xy + a.yy*b.yy + a.yz*b.zy + a.yw*b.wy,
            a.yx*b.xz + a.yy*b.yz + a.yz*b.zz + a.yw*b.wz,
            a.yx*b.xw + a.yy*b.yw + a.yz*b.zw + a.yw*b.ww
        };
    }
    
    template <typename T, typename U>
    requires trait::self_mul_v<T,U>
    Ten24<T>& operator*=(Ten24<T>&a, const Ten44<U>& b)
    {
        a = a * b;
        return a;
    }

    template <typename T>
    constexpr Ten13<T>  transpose(const Ten31<T>&v)
    {
        return {
            v.xx, v.yx, v.zx
        };
    }

    template <typename T, typename U>
    constexpr Ten31<product_t<T,U>> operator*(const Ten31<T>& a, const Ten11<U>& b)
    {
        return {
            a.xx*b.xx,

            a.yx*b.xx,

            a.zx*b.xx
        };
    }
    
    template <typename T, typename U>
    requires trait::self_mul_v<T,U>
    Ten31<T>& operator*=(Ten31<T>&a, const Ten11<U>& b)
    {
        a = a * b;
        return a;
    }

    template <typename T, typename U>
    constexpr Ten32<product_t<T,U>> operator*(const Ten31<T>& a, const Ten12<U>& b)
    {
        return {
            a.xx*b.xx,
            a.xx*b.xy,

            a.yx*b.xx,
            a.yx*b.xy,

            a.zx*b.xx,
            a.zx*b.xy
        };
    }
    
    template <typename T, typename U>
    constexpr Ten33<product_t<T,U>> operator*(const Ten31<T>& a, const Ten13<U>& b)
    {
        return {
            a.xx*b.xx,
            a.xx*b.xy,
            a.xx*b.xz,

            a.yx*b.xx,
            a.yx*b.xy,
            a.yx*b.xz,

            a.zx*b.xx,
            a.zx*b.xy,
            a.zx*b.xz
        };
    }
    
    template <typename T, typename U>
    constexpr Ten34<product_t<T,U>> operator*(const Ten31<T>& a, const Ten14<U>& b)
    {
        return {
            a.xx*b.xx,
            a.xx*b.xy,
            a.xx*b.xz,
            a.xx*b.xw,

            a.yx*b.xx,
            a.yx*b.xy,
            a.yx*b.xz,
            a.yx*b.xw,

            a.zx*b.xx,
            a.zx*b.xy,
            a.zx*b.xz,
            a.zx*b.xw
        };
    }
    
    template <typename T>
    constexpr Ten23<T>  transpose(const Ten32<T>&v)
    {
        return {
            v.xx, v.yx, v.zx,
            v.xy, v.yy, v.zy
        };
    }

    template <typename T, typename U>
    constexpr Ten31<product_t<T,U>> operator*(const Ten32<T>& a, const Ten21<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx,

            a.yx*b.xx + a.yy*b.yx,

            a.zx*b.xx + a.zy*b.yx
        };
    }
    
    template <typename T, typename U>
    constexpr Ten32<product_t<T,U>> operator*(const Ten32<T>& a, const Ten22<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx,
            a.xx*b.xy + a.xy*b.yy,

            a.yx*b.xx + a.yy*b.yx,
            a.yx*b.xy + a.yy*b.yy,

            a.zx*b.xx + a.zy*b.yx,
            a.zx*b.xy + a.zy*b.yy
        };
    }
    
    template <typename T, typename U>
    requires trait::self_mul_v<T,U>
    Ten32<T>& operator*=(Ten32<T>&a, const Ten22<U>& b)
    {
        a = a * b;
        return a;
    }

    template <typename T, typename U>
    constexpr Ten33<product_t<T,U>> operator*(const Ten32<T>& a, const Ten23<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx,
            a.xx*b.xy + a.xy*b.yy,
            a.xx*b.xz + a.xy*b.yz,

            a.yx*b.xx + a.yy*b.yx,
            a.yx*b.xy + a.yy*b.yy,
            a.yx*b.xz + a.yy*b.yz,

            a.zx*b.xx + a.zy*b.yx,
            a.zx*b.xy + a.zy*b.yy,
            a.zx*b.xz + a.zy*b.yz
        };
    }
    
    template <typename T, typename U>
    constexpr Ten34<product_t<T,U>> operator*(const Ten32<T>& a, const Ten24<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx,
            a.xx*b.xy + a.xy*b.yy,
            a.xx*b.xz + a.xy*b.yz,
            a.xx*b.xw + a.xy*b.yw,

            a.yx*b.xx + a.yy*b.yx,
            a.yx*b.xy + a.yy*b.yy,
            a.yx*b.xz + a.yy*b.yz,
            a.yx*b.xw + a.yy*b.yw,

            a.zx*b.xx + a.zy*b.yx,
            a.zx*b.xy + a.zy*b.yy,
            a.zx*b.xz + a.zy*b.yz,
            a.zx*b.xw + a.zy*b.yw
        };
    }
    
    template <typename T, typename U>
    constexpr Ten31<product_t<T,U>> operator*(const Ten33<T>& a, const Ten31<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx,

            a.yx*b.xx + a.yy*b.yx + a.yz*b.zx,

            a.zx*b.xx + a.zy*b.yx + a.zz*b.zx
        };
    }
    
    template <typename T, typename U>
    constexpr Ten32<product_t<T,U>> operator*(const Ten33<T>& a, const Ten32<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx,
            a.xx*b.xy + a.xy*b.yy + a.xz*b.zy,

            a.yx*b.xx + a.yy*b.yx + a.yz*b.zx,
            a.yx*b.xy + a.yy*b.yy + a.yz*b.zy,

            a.zx*b.xx + a.zy*b.yx + a.zz*b.zx,
            a.zx*b.xy + a.zy*b.yy + a.zz*b.zy
        };
    }
    
    template <typename T, typename U>
    constexpr Ten34<product_t<T,U>> operator*(const Ten33<T>& a, const Ten34<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx,
            a.xx*b.xy + a.xy*b.yy + a.xz*b.zy,
            a.xx*b.xz + a.xy*b.yz + a.xz*b.zz,
            a.xx*b.xw + a.xy*b.yw + a.xz*b.zw,

            a.yx*b.xx + a.yy*b.yx + a.yz*b.zx,
            a.yx*b.xy + a.yy*b.yy + a.yz*b.zy,
            a.yx*b.xz + a.yy*b.yz + a.yz*b.zz,
            a.yx*b.xw + a.yy*b.yw + a.yz*b.zw,

            a.zx*b.xx + a.zy*b.yx + a.zz*b.zx,
            a.zx*b.xy + a.zy*b.yy + a.zz*b.zy,
            a.zx*b.xz + a.zy*b.yz + a.zz*b.zz,
            a.zx*b.xw + a.zy*b.yw + a.zz*b.zw
        };
    }
    
    template <typename T>
    constexpr Ten43<T>  transpose(const Ten34<T>&v)
    {
        return {
            v.xx, v.yx, v.zx,
            v.xy, v.yy, v.zy,
            v.xz, v.yz, v.zz,
            v.xw, v.yw, v.zw
        };
    }

    template <typename T, typename U>
    constexpr Ten31<product_t<T,U>> operator*(const Ten34<T>& a, const Ten41<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx + a.xw*b.wx,

            a.yx*b.xx + a.yy*b.yx + a.yz*b.zx + a.yw*b.wx,

            a.zx*b.xx + a.zy*b.yx + a.zz*b.zx + a.zw*b.wx
        };
    }
    
    template <typename T, typename U>
    constexpr Ten32<product_t<T,U>> operator*(const Ten34<T>& a, const Ten42<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx + a.xw*b.wx,
            a.xx*b.xy + a.xy*b.yy + a.xz*b.zy + a.xw*b.wy,

            a.yx*b.xx + a.yy*b.yx + a.yz*b.zx + a.yw*b.wx,
            a.yx*b.xy + a.yy*b.yy + a.yz*b.zy + a.yw*b.wy,

            a.zx*b.xx + a.zy*b.yx + a.zz*b.zx + a.zw*b.wx,
            a.zx*b.xy + a.zy*b.yy + a.zz*b.zy + a.zw*b.wy
        };
    }
    
    template <typename T, typename U>
    constexpr Ten33<product_t<T,U>> operator*(const Ten34<T>& a, const Ten43<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx + a.xw*b.wx,
            a.xx*b.xy + a.xy*b.yy + a.xz*b.zy + a.xw*b.wy,
            a.xx*b.xz + a.xy*b.yz + a.xz*b.zz + a.xw*b.wz,

            a.yx*b.xx + a.yy*b.yx + a.yz*b.zx + a.yw*b.wx,
            a.yx*b.xy + a.yy*b.yy + a.yz*b.zy + a.yw*b.wy,
            a.yx*b.xz + a.yy*b.yz + a.yz*b.zz + a.yw*b.wz,

            a.zx*b.xx + a.zy*b.yx + a.zz*b.zx + a.zw*b.wx,
            a.zx*b.xy + a.zy*b.yy + a.zz*b.zy + a.zw*b.wy,
            a.zx*b.xz + a.zy*b.yz + a.zz*b.zz + a.zw*b.wz
        };
    }
    
    template <typename T, typename U>
    constexpr Ten34<product_t<T,U>> operator*(const Ten34<T>& a, const Ten44<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx + a.xw*b.wx,
            a.xx*b.xy + a.xy*b.yy + a.xz*b.zy + a.xw*b.wy,
            a.xx*b.xz + a.xy*b.yz + a.xz*b.zz + a.xw*b.wz,
            a.xx*b.xw + a.xy*b.yw + a.xz*b.zw + a.xw*b.ww,

            a.yx*b.xx + a.yy*b.yx + a.yz*b.zx + a.yw*b.wx,
            a.yx*b.xy + a.yy*b.yy + a.yz*b.zy + a.yw*b.wy,
            a.yx*b.xz + a.yy*b.yz + a.yz*b.zz + a.yw*b.wz,
            a.yx*b.xw + a.yy*b.yw + a.yz*b.zw + a.yw*b.ww,

            a.zx*b.xx + a.zy*b.yx + a.zz*b.zx + a.zw*b.wx,
            a.zx*b.xy + a.zy*b.yy + a.zz*b.zy + a.zw*b.wy,
            a.zx*b.xz + a.zy*b.yz + a.zz*b.zz + a.zw*b.wz,
            a.zx*b.xw + a.zy*b.yw + a.zz*b.zw + a.zw*b.ww
        };
    }
    
    template <typename T, typename U>
    requires trait::self_mul_v<T,U>
    Ten34<T>& operator*=(Ten34<T>&a, const Ten44<U>& b)
    {
        a = a * b;
        return a;
    }

    template <typename T>
    constexpr Ten14<T>  transpose(const Ten41<T>&v)
    {
        return {
            v.xx, v.yx, v.zx, v.wx
        };
    }

    template <typename T, typename U>
    constexpr Ten41<product_t<T,U>> operator*(const Ten41<T>& a, const Ten11<U>& b)
    {
        return {
            a.xx*b.xx,

            a.yx*b.xx,

            a.zx*b.xx,

            a.wx*b.xx
        };
    }
    
    template <typename T, typename U>
    requires trait::self_mul_v<T,U>
    Ten41<T>& operator*=(Ten41<T>&a, const Ten11<U>& b)
    {
        a = a * b;
        return a;
    }

    template <typename T, typename U>
    constexpr Ten42<product_t<T,U>> operator*(const Ten41<T>& a, const Ten12<U>& b)
    {
        return {
            a.xx*b.xx,
            a.xx*b.xy,

            a.yx*b.xx,
            a.yx*b.xy,

            a.zx*b.xx,
            a.zx*b.xy,

            a.wx*b.xx,
            a.wx*b.xy
        };
    }
    
    template <typename T, typename U>
    constexpr Ten43<product_t<T,U>> operator*(const Ten41<T>& a, const Ten13<U>& b)
    {
        return {
            a.xx*b.xx,
            a.xx*b.xy,
            a.xx*b.xz,

            a.yx*b.xx,
            a.yx*b.xy,
            a.yx*b.xz,

            a.zx*b.xx,
            a.zx*b.xy,
            a.zx*b.xz,

            a.wx*b.xx,
            a.wx*b.xy,
            a.wx*b.xz
        };
    }
    
    template <typename T, typename U>
    constexpr Ten44<product_t<T,U>> operator*(const Ten41<T>& a, const Ten14<U>& b)
    {
        return {
            a.xx*b.xx,
            a.xx*b.xy,
            a.xx*b.xz,
            a.xx*b.xw,

            a.yx*b.xx,
            a.yx*b.xy,
            a.yx*b.xz,
            a.yx*b.xw,

            a.zx*b.xx,
            a.zx*b.xy,
            a.zx*b.xz,
            a.zx*b.xw,

            a.wx*b.xx,
            a.wx*b.xy,
            a.wx*b.xz,
            a.wx*b.xw
        };
    }
    
    template <typename T>
    constexpr Ten24<T>  transpose(const Ten42<T>&v)
    {
        return {
            v.xx, v.yx, v.zx, v.wx,
            v.xy, v.yy, v.zy, v.wy
        };
    }

    template <typename T, typename U>
    constexpr Ten41<product_t<T,U>> operator*(const Ten42<T>& a, const Ten21<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx,

            a.yx*b.xx + a.yy*b.yx,

            a.zx*b.xx + a.zy*b.yx,

            a.wx*b.xx + a.wy*b.yx
        };
    }
    
    template <typename T, typename U>
    constexpr Ten42<product_t<T,U>> operator*(const Ten42<T>& a, const Ten22<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx,
            a.xx*b.xy + a.xy*b.yy,

            a.yx*b.xx + a.yy*b.yx,
            a.yx*b.xy + a.yy*b.yy,

            a.zx*b.xx + a.zy*b.yx,
            a.zx*b.xy + a.zy*b.yy,

            a.wx*b.xx + a.wy*b.yx,
            a.wx*b.xy + a.wy*b.yy
        };
    }
    
    template <typename T, typename U>
    requires trait::self_mul_v<T,U>
    Ten42<T>& operator*=(Ten42<T>&a, const Ten22<U>& b)
    {
        a = a * b;
        return a;
    }

    template <typename T, typename U>
    constexpr Ten43<product_t<T,U>> operator*(const Ten42<T>& a, const Ten23<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx,
            a.xx*b.xy + a.xy*b.yy,
            a.xx*b.xz + a.xy*b.yz,

            a.yx*b.xx + a.yy*b.yx,
            a.yx*b.xy + a.yy*b.yy,
            a.yx*b.xz + a.yy*b.yz,

            a.zx*b.xx + a.zy*b.yx,
            a.zx*b.xy + a.zy*b.yy,
            a.zx*b.xz + a.zy*b.yz,

            a.wx*b.xx + a.wy*b.yx,
            a.wx*b.xy + a.wy*b.yy,
            a.wx*b.xz + a.wy*b.yz
        };
    }
    
    template <typename T, typename U>
    constexpr Ten44<product_t<T,U>> operator*(const Ten42<T>& a, const Ten24<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx,
            a.xx*b.xy + a.xy*b.yy,
            a.xx*b.xz + a.xy*b.yz,
            a.xx*b.xw + a.xy*b.yw,

            a.yx*b.xx + a.yy*b.yx,
            a.yx*b.xy + a.yy*b.yy,
            a.yx*b.xz + a.yy*b.yz,
            a.yx*b.xw + a.yy*b.yw,

            a.zx*b.xx + a.zy*b.yx,
            a.zx*b.xy + a.zy*b.yy,
            a.zx*b.xz + a.zy*b.yz,
            a.zx*b.xw + a.zy*b.yw,

            a.wx*b.xx + a.wy*b.yx,
            a.wx*b.xy + a.wy*b.yy,
            a.wx*b.xz + a.wy*b.yz,
            a.wx*b.xw + a.wy*b.yw
        };
    }
    
    template <typename T>
    constexpr Ten34<T>  transpose(const Ten43<T>&v)
    {
        return {
            v.xx, v.yx, v.zx, v.wx,
            v.xy, v.yy, v.zy, v.wy,
            v.xz, v.yz, v.zz, v.wz
        };
    }

    template <typename T, typename U>
    constexpr Ten41<product_t<T,U>> operator*(const Ten43<T>& a, const Ten31<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx,

            a.yx*b.xx + a.yy*b.yx + a.yz*b.zx,

            a.zx*b.xx + a.zy*b.yx + a.zz*b.zx,

            a.wx*b.xx + a.wy*b.yx + a.wz*b.zx
        };
    }
    
    template <typename T, typename U>
    constexpr Ten42<product_t<T,U>> operator*(const Ten43<T>& a, const Ten32<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx,
            a.xx*b.xy + a.xy*b.yy + a.xz*b.zy,

            a.yx*b.xx + a.yy*b.yx + a.yz*b.zx,
            a.yx*b.xy + a.yy*b.yy + a.yz*b.zy,

            a.zx*b.xx + a.zy*b.yx + a.zz*b.zx,
            a.zx*b.xy + a.zy*b.yy + a.zz*b.zy,

            a.wx*b.xx + a.wy*b.yx + a.wz*b.zx,
            a.wx*b.xy + a.wy*b.yy + a.wz*b.zy
        };
    }
    
    template <typename T, typename U>
    constexpr Ten43<product_t<T,U>> operator*(const Ten43<T>& a, const Ten33<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx,
            a.xx*b.xy + a.xy*b.yy + a.xz*b.zy,
            a.xx*b.xz + a.xy*b.yz + a.xz*b.zz,

            a.yx*b.xx + a.yy*b.yx + a.yz*b.zx,
            a.yx*b.xy + a.yy*b.yy + a.yz*b.zy,
            a.yx*b.xz + a.yy*b.yz + a.yz*b.zz,

            a.zx*b.xx + a.zy*b.yx + a.zz*b.zx,
            a.zx*b.xy + a.zy*b.yy + a.zz*b.zy,
            a.zx*b.xz + a.zy*b.yz + a.zz*b.zz,

            a.wx*b.xx + a.wy*b.yx + a.wz*b.zx,
            a.wx*b.xy + a.wy*b.yy + a.wz*b.zy,
            a.wx*b.xz + a.wy*b.yz + a.wz*b.zz
        };
    }
    
    template <typename T, typename U>
    requires trait::self_mul_v<T,U>
    Ten43<T>& operator*=(Ten43<T>&a, const Ten33<U>& b)
    {
        a = a * b;
        return a;
    }

    template <typename T, typename U>
    constexpr Ten44<product_t<T,U>> operator*(const Ten43<T>& a, const Ten34<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx,
            a.xx*b.xy + a.xy*b.yy + a.xz*b.zy,
            a.xx*b.xz + a.xy*b.yz + a.xz*b.zz,
            a.xx*b.xw + a.xy*b.yw + a.xz*b.zw,

            a.yx*b.xx + a.yy*b.yx + a.yz*b.zx,
            a.yx*b.xy + a.yy*b.yy + a.yz*b.zy,
            a.yx*b.xz + a.yy*b.yz + a.yz*b.zz,
            a.yx*b.xw + a.yy*b.yw + a.yz*b.zw,

            a.zx*b.xx + a.zy*b.yx + a.zz*b.zx,
            a.zx*b.xy + a.zy*b.yy + a.zz*b.zy,
            a.zx*b.xz + a.zy*b.yz + a.zz*b.zz,
            a.zx*b.xw + a.zy*b.yw + a.zz*b.zw,

            a.wx*b.xx + a.wy*b.yx + a.wz*b.zx,
            a.wx*b.xy + a.wy*b.yy + a.wz*b.zy,
            a.wx*b.xz + a.wy*b.yz + a.wz*b.zz,
            a.wx*b.xw + a.wy*b.yw + a.wz*b.zw
        };
    }
    
    template <typename T, typename U>
    constexpr Ten41<product_t<T,U>> operator*(const Ten44<T>& a, const Ten41<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx + a.xw*b.wx,

            a.yx*b.xx + a.yy*b.yx + a.yz*b.zx + a.yw*b.wx,

            a.zx*b.xx + a.zy*b.yx + a.zz*b.zx + a.zw*b.wx,

            a.wx*b.xx + a.wy*b.yx + a.wz*b.zx + a.ww*b.wx
        };
    }
    
    template <typename T, typename U>
    constexpr Ten42<product_t<T,U>> operator*(const Ten44<T>& a, const Ten42<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx + a.xw*b.wx,
            a.xx*b.xy + a.xy*b.yy + a.xz*b.zy + a.xw*b.wy,

            a.yx*b.xx + a.yy*b.yx + a.yz*b.zx + a.yw*b.wx,
            a.yx*b.xy + a.yy*b.yy + a.yz*b.zy + a.yw*b.wy,

            a.zx*b.xx + a.zy*b.yx + a.zz*b.zx + a.zw*b.wx,
            a.zx*b.xy + a.zy*b.yy + a.zz*b.zy + a.zw*b.wy,

            a.wx*b.xx + a.wy*b.yx + a.wz*b.zx + a.ww*b.wx,
            a.wx*b.xy + a.wy*b.yy + a.wz*b.zy + a.ww*b.wy
        };
    }
    
    template <typename T, typename U>
    constexpr Ten43<product_t<T,U>> operator*(const Ten44<T>& a, const Ten43<U>& b)
    {
        return {
            a.xx*b.xx + a.xy*b.yx + a.xz*b.zx + a.xw*b.wx,
            a.xx*b.xy + a.xy*b.yy + a.xz*b.zy + a.xw*b.wy,
            a.xx*b.xz + a.xy*b.yz + a.xz*b.zz + a.xw*b.wz,

            a.yx*b.xx + a.yy*b.yx + a.yz*b.zx + a.yw*b.wx,
            a.yx*b.xy + a.yy*b.yy + a.yz*b.zy + a.yw*b.wy,
            a.yx*b.xz + a.yy*b.yz + a.yz*b.zz + a.yw*b.wz,

            a.zx*b.xx + a.zy*b.yx + a.zz*b.zx + a.zw*b.wx,
            a.zx*b.xy + a.zy*b.yy + a.zz*b.zy + a.zw*b.wy,
            a.zx*b.xz + a.zy*b.yz + a.zz*b.zz + a.zw*b.wz,

            a.wx*b.xx + a.wy*b.yx + a.wz*b.zx + a.ww*b.wx,
            a.wx*b.xy + a.wy*b.yy + a.wz*b.zy + a.ww*b.wy,
            a.wx*b.xz + a.wy*b.yz + a.wz*b.zz + a.ww*b.wz
        };
    }
    }
