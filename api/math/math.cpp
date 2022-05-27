////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <math/preamble.hpp>
#include <math/Coord2.hpp>
#include <math/Fraction.hpp>
#include <math/shape/Size2.hpp>

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
);

namespace yq {
    void        initialize_math()
    {
    }
}
