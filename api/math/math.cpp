////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <math/preamble.hpp>
#include <math/Coord2.hpp>
#include <math/Fraction.hpp>
#include <math/shape/Size2.hpp>
#include <math/vec/Vec1.hpp>
#include <math/vec/Vec2.hpp>
#include <math/vec/Vec3.hpp>
#include <math/vec/Vec4.hpp>

#include <basic/DelayInit.hpp>
#include <basic/meta/Init.hpp>

using namespace yq;

YQ_TYPE_IMPLEMENT(yq::Coord2D)
YQ_TYPE_IMPLEMENT(yq::Coord2F)
YQ_TYPE_IMPLEMENT(yq::Coord2I)
YQ_TYPE_IMPLEMENT(yq::Coord2U)

YQ_TYPE_IMPLEMENT(yq::Frac8)
YQ_TYPE_IMPLEMENT(yq::Frac16)
YQ_TYPE_IMPLEMENT(yq::Frac32)
YQ_TYPE_IMPLEMENT(yq::Frac64)

YQ_TYPE_IMPLEMENT(yq::Size2D)
YQ_TYPE_IMPLEMENT(yq::Size2F)
YQ_TYPE_IMPLEMENT(yq::Size2I)
YQ_TYPE_IMPLEMENT(yq::Size2U)

YQ_TYPE_IMPLEMENT(yq::Vec1D)
YQ_TYPE_IMPLEMENT(yq::Vec1F)
YQ_TYPE_IMPLEMENT(yq::Vec1I)
YQ_TYPE_IMPLEMENT(yq::Vec1U)

YQ_TYPE_IMPLEMENT(yq::Vec2D)
YQ_TYPE_IMPLEMENT(yq::Vec2F)
YQ_TYPE_IMPLEMENT(yq::Vec2I)
YQ_TYPE_IMPLEMENT(yq::Vec2U)

YQ_TYPE_IMPLEMENT(yq::Vec3D)
YQ_TYPE_IMPLEMENT(yq::Vec3F)
YQ_TYPE_IMPLEMENT(yq::Vec3I)
YQ_TYPE_IMPLEMENT(yq::Vec3U)

YQ_TYPE_IMPLEMENT(yq::Vec4D)
YQ_TYPE_IMPLEMENT(yq::Vec4F)
YQ_TYPE_IMPLEMENT(yq::Vec4I)
YQ_TYPE_IMPLEMENT(yq::Vec4U)

YQ_INVOKE(
    auto coord2d = writer<Coord2D>();
    coord2d.property("i", &Coord2D::i);
    coord2d.property("j", &Coord2D::j);

    auto coord2f = writer<Coord2F>();
    coord2f.property("i", &Coord2F::i);
    coord2f.property("j", &Coord2F::j);

    auto coord2i = writer<Coord2I>();
    coord2i.property("i", &Coord2I::i);
    coord2i.property("j", &Coord2I::j);

    auto coord2u = writer<Coord2U>();
    coord2u.property("i", &Coord2U::i);
    coord2u.property("j", &Coord2U::j);

    auto frac8  = writer<Frac8>();
    frac8.property("num", &Frac8::num);
    frac8.property("den", &Frac8::den);
    
    auto frac16 = writer<Frac16>();
    frac16.property("num", &Frac16::num);
    frac16.property("den", &Frac16::den);

    auto frac32 = writer<Frac32>();
    frac32.property("num", &Frac32::num);
    frac32.property("den", &Frac32::den);

    auto frac64 = writer<Frac64>();
    frac64.property("num", &Frac64::num);
    frac64.property("den", &Frac64::den);
    
    auto size2d = writer<Size2D>();
    size2d.property("x", &Size2D::x);
    size2d.property("y", &Size2D::y);
    size2d.property("width", &Size2D::width);
    size2d.property("height", &Size2D::height);

    auto size2f = writer<Size2F>();
    size2f.property("x", &Size2F::x);
    size2f.property("y", &Size2F::y);
    size2f.property("width", &Size2F::width);
    size2f.property("height", &Size2F::height);

    auto size2i = writer<Size2I>();
    size2i.property("x", &Size2I::x);
    size2i.property("y", &Size2I::y);
    size2i.property("width", &Size2I::width);
    size2i.property("height", &Size2I::height);

    auto size2u = writer<Size2U>();
    size2u.property("x", &Size2U::x);
    size2u.property("y", &Size2U::y);
    size2u.property("width", &Size2U::width);
    size2u.property("height", &Size2U::height);
    
    auto vec1d = writer<Vec1D>();
    vec1d.property("x", &Vec1D::x);

    auto vec1f = writer<Vec1F>();
    vec1f.property("x", &Vec1F::x);

    auto vec1i = writer<Vec1I>();
    vec1i.property("x", &Vec1I::x);

    auto vec1u = writer<Vec1U>();
    vec1u.property("x", &Vec1U::x);
    
    auto vec2d = writer<Vec2D>();
    vec2d.property("x", &Vec2D::x);
    vec2d.property("y", &Vec2D::y);

    auto vec2f = writer<Vec2F>();
    vec2f.property("x", &Vec2F::x);
    vec2f.property("y", &Vec2F::y);

    auto vec2i = writer<Vec2I>();
    vec2i.property("x", &Vec2I::x);
    vec2i.property("y", &Vec2I::y);

    auto vec2u = writer<Vec2U>();
    vec2u.property("x", &Vec2U::x);
    vec2u.property("y", &Vec2U::y);

    auto vec3d = writer<Vec3D>();
    vec3d.property("x", &Vec3D::x);
    vec3d.property("y", &Vec3D::y);
    vec3d.property("z", &Vec3D::z);

    auto vec3f = writer<Vec3F>();
    vec3f.property("x", &Vec3F::x);
    vec3f.property("y", &Vec3F::y);
    vec3f.property("z", &Vec3F::z);

    auto vec3i = writer<Vec3I>();
    vec3i.property("x", &Vec3I::x);
    vec3i.property("y", &Vec3I::y);
    vec3i.property("z", &Vec3I::z);

    auto vec3u = writer<Vec3U>();
    vec3u.property("x", &Vec3U::x);
    vec3u.property("y", &Vec3U::y);
    vec3u.property("z", &Vec3U::z);

    auto vec4d = writer<Vec4D>();
    vec4d.property("x", &Vec4D::x);
    vec4d.property("y", &Vec4D::y);
    vec4d.property("z", &Vec4D::z);
    vec4d.property("w", &Vec4D::w);

    auto vec4f = writer<Vec4F>();
    vec4f.property("x", &Vec4F::x);
    vec4f.property("y", &Vec4F::y);
    vec4f.property("z", &Vec4F::z);
    vec4f.property("w", &Vec4F::w);

    auto vec4i = writer<Vec4I>();
    vec4i.property("x", &Vec4I::x);
    vec4i.property("y", &Vec4I::y);
    vec4i.property("z", &Vec4I::z);
    vec4i.property("w", &Vec4I::w);

    auto vec4u = writer<Vec4U>();
    vec4u.property("x", &Vec4U::x);
    vec4u.property("y", &Vec4U::y);
    vec4u.property("z", &Vec4U::z);
    vec4u.property("w", &Vec4U::w);

);

namespace yq {
    void        initialize_math()
    {
    }
}
