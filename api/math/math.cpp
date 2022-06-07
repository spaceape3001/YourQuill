////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <math/preamble.hpp>
#include <math/Complex.hpp>
#include <math/ColorRgb.hpp>
#include <math/ColorRgba.hpp>
#include <math/Coord2.hpp>
#include <math/Fraction.hpp>

#include <math/shape/AxBox1.hpp>
#include <math/shape/AxBox2.hpp>
#include <math/shape/AxBox3.hpp>
#include <math/shape/AxBox4.hpp>

#include <math/shape/Seg1.hpp>
#include <math/shape/Seg2.hpp>
#include <math/shape/Seg3.hpp>
#include <math/shape/Seg4.hpp>

#include <math/shape/Size2.hpp>

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

#include <math/vec/Vec1.hpp>
#include <math/vec/Vec2.hpp>
#include <math/vec/Vec3.hpp>
#include <math/vec/Vec4.hpp>

#include <basic/DelayInit.hpp>
#include <basic/meta/Init.hpp>

using namespace yq;

YQ_TYPE_IMPLEMENT(yq::AxBox1D)
YQ_TYPE_IMPLEMENT(yq::AxBox1F)
YQ_TYPE_IMPLEMENT(yq::AxBox1I)
YQ_TYPE_IMPLEMENT(yq::AxBox1U)

YQ_TYPE_IMPLEMENT(yq::AxBox2D)
YQ_TYPE_IMPLEMENT(yq::AxBox2F)
YQ_TYPE_IMPLEMENT(yq::AxBox2I)
YQ_TYPE_IMPLEMENT(yq::AxBox2U)

YQ_TYPE_IMPLEMENT(yq::AxBox3D)
YQ_TYPE_IMPLEMENT(yq::AxBox3F)
YQ_TYPE_IMPLEMENT(yq::AxBox3I)
YQ_TYPE_IMPLEMENT(yq::AxBox3U)

YQ_TYPE_IMPLEMENT(yq::AxBox4D)
YQ_TYPE_IMPLEMENT(yq::AxBox4F)
YQ_TYPE_IMPLEMENT(yq::AxBox4I)
YQ_TYPE_IMPLEMENT(yq::AxBox4U)

YQ_TYPE_IMPLEMENT(yq::ComplexD)
YQ_TYPE_IMPLEMENT(yq::ComplexF)
YQ_TYPE_IMPLEMENT(yq::ComplexI)
YQ_TYPE_IMPLEMENT(yq::ComplexU)

YQ_TYPE_IMPLEMENT(yq::Coord2D)
YQ_TYPE_IMPLEMENT(yq::Coord2F)
YQ_TYPE_IMPLEMENT(yq::Coord2I)
YQ_TYPE_IMPLEMENT(yq::Coord2U)

YQ_TYPE_IMPLEMENT(yq::Frac8)
YQ_TYPE_IMPLEMENT(yq::Frac16)
YQ_TYPE_IMPLEMENT(yq::Frac32)
YQ_TYPE_IMPLEMENT(yq::Frac64)

YQ_TYPE_IMPLEMENT(yq::Seg1D)
YQ_TYPE_IMPLEMENT(yq::Seg1F)
YQ_TYPE_IMPLEMENT(yq::Seg1I)
YQ_TYPE_IMPLEMENT(yq::Seg1U)

YQ_TYPE_IMPLEMENT(yq::Seg2D)
YQ_TYPE_IMPLEMENT(yq::Seg2F)
YQ_TYPE_IMPLEMENT(yq::Seg2I)
YQ_TYPE_IMPLEMENT(yq::Seg2U)

YQ_TYPE_IMPLEMENT(yq::Seg3D)
YQ_TYPE_IMPLEMENT(yq::Seg3F)
YQ_TYPE_IMPLEMENT(yq::Seg3I)
YQ_TYPE_IMPLEMENT(yq::Seg3U)

YQ_TYPE_IMPLEMENT(yq::Seg4D)
YQ_TYPE_IMPLEMENT(yq::Seg4F)
YQ_TYPE_IMPLEMENT(yq::Seg4I)
YQ_TYPE_IMPLEMENT(yq::Seg4U)

YQ_TYPE_IMPLEMENT(yq::Size2D)
YQ_TYPE_IMPLEMENT(yq::Size2F)
YQ_TYPE_IMPLEMENT(yq::Size2I)
YQ_TYPE_IMPLEMENT(yq::Size2U)

YQ_INVOKE(


    auto axbox1d = writer<AxBox1D>();
    axbox1d.property("lo", &AxBox1D::lo);
    axbox1d.property("hi", &AxBox1D::hi);

    auto axbox1f = writer<AxBox1F>();
    axbox1f.property("lo", &AxBox1F::lo);
    axbox1f.property("hi", &AxBox1F::hi);

    auto axbox1i = writer<AxBox1I>();
    axbox1i.property("lo", &AxBox1I::lo);
    axbox1i.property("hi", &AxBox1I::hi);

    auto axbox1u = writer<AxBox1U>();
    axbox1u.property("lo", &AxBox1U::lo);
    axbox1u.property("hi", &AxBox1U::hi);


    auto axbox2d = writer<AxBox2D>();
    axbox2d.property("lo", &AxBox2D::lo);
    axbox2d.property("hi", &AxBox2D::hi);

    auto axbox2f = writer<AxBox2F>();
    axbox2f.property("lo", &AxBox2F::lo);
    axbox2f.property("hi", &AxBox2F::hi);

    auto axbox2i = writer<AxBox2I>();
    axbox2i.property("lo", &AxBox2I::lo);
    axbox2i.property("hi", &AxBox2I::hi);

    auto axbox2u = writer<AxBox2U>();
    axbox2u.property("lo", &AxBox2U::lo);
    axbox2u.property("hi", &AxBox2U::hi);


    auto axbox3d = writer<AxBox3D>();
    axbox3d.property("lo", &AxBox3D::lo);
    axbox3d.property("hi", &AxBox3D::hi);

    auto axbox3f = writer<AxBox3F>();
    axbox3f.property("lo", &AxBox3F::lo);
    axbox3f.property("hi", &AxBox3F::hi);

    auto axbox3i = writer<AxBox3I>();
    axbox3i.property("lo", &AxBox3I::lo);
    axbox3i.property("hi", &AxBox3I::hi);

    auto axbox3u = writer<AxBox3U>();
    axbox3u.property("lo", &AxBox3U::lo);
    axbox3u.property("hi", &AxBox3U::hi);


    auto axbox4d = writer<AxBox4D>();
    axbox4d.property("lo", &AxBox4D::lo);
    axbox4d.property("hi", &AxBox4D::hi);

    auto axbox4f = writer<AxBox4F>();
    axbox4f.property("lo", &AxBox4F::lo);
    axbox4f.property("hi", &AxBox4F::hi);

    auto axbox4i = writer<AxBox4I>();
    axbox4i.property("lo", &AxBox4I::lo);
    axbox4i.property("hi", &AxBox4I::hi);

    auto axbox4u = writer<AxBox4U>();
    axbox4u.property("lo", &AxBox4U::lo);
    axbox4u.property("hi", &AxBox4U::hi);


    auto complexD = writer<ComplexD>();
    complexD.property("re", (double (ComplexD::*)() const) &ComplexD::real).setter((void(ComplexD::*)(double)) &ComplexD::real);
    complexD.property("im", (double (ComplexD::*)() const) &ComplexD::imag).setter((void(ComplexD::*)(double)) &ComplexD::imag);

    auto complexF = writer<ComplexF>();
    complexF.property("re", (float (ComplexF::*)() const) &ComplexF::real).setter((void(ComplexF::*)(float)) &ComplexF::real);
    complexF.property("im", (float (ComplexF::*)() const) &ComplexF::imag).setter((void(ComplexF::*)(float)) &ComplexF::imag);

    auto complexI = writer<ComplexI>();
    complexI.property("re", (int (ComplexI::*)() const) &ComplexI::real).setter((void(ComplexI::*)(int)) &ComplexI::real);
    complexI.property("im", (int (ComplexI::*)() const) &ComplexI::imag).setter((void(ComplexI::*)(int)) &ComplexI::imag);

    auto complexU = writer<ComplexU>();
    complexU.property("re", (unsigned (ComplexU::*)() const) &ComplexU::real).setter((void(ComplexU::*)(unsigned)) &ComplexU::real);
    complexU.property("im", (unsigned (ComplexU::*)() const) &ComplexU::imag).setter((void(ComplexU::*)(unsigned)) &ComplexU::imag);


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
    


    auto seg1d = writer<Seg1D>();
    seg1d.property("a", &Seg1D::a);
    seg1d.property("b", &Seg1D::b);

    auto seg1f = writer<Seg1F>();
    seg1f.property("a", &Seg1F::a);
    seg1f.property("b", &Seg1F::b);

    auto seg1i = writer<Seg1I>();
    seg1i.property("a", &Seg1I::a);
    seg1i.property("b", &Seg1I::b);

    auto seg1u = writer<Seg1U>();
    seg1u.property("a", &Seg1U::a);
    seg1u.property("b", &Seg1U::b);


    auto seg2d = writer<Seg2D>();
    seg2d.property("a", &Seg2D::a);
    seg2d.property("b", &Seg2D::b);

    auto seg2f = writer<Seg2F>();
    seg2f.property("a", &Seg2F::a);
    seg2f.property("b", &Seg2F::b);

    auto seg2i = writer<Seg2I>();
    seg2i.property("a", &Seg2I::a);
    seg2i.property("b", &Seg2I::b);

    auto seg2u = writer<Seg2U>();
    seg2u.property("a", &Seg2U::a);
    seg2u.property("b", &Seg2U::b);


    auto seg3d = writer<Seg3D>();
    seg3d.property("a", &Seg3D::a);
    seg3d.property("b", &Seg3D::b);

    auto seg3f = writer<Seg3F>();
    seg3f.property("a", &Seg3F::a);
    seg3f.property("b", &Seg3F::b);

    auto seg3i = writer<Seg3I>();
    seg3i.property("a", &Seg3I::a);
    seg3i.property("b", &Seg3I::b);

    auto seg3u = writer<Seg3U>();
    seg3u.property("a", &Seg3U::a);
    seg3u.property("b", &Seg3U::b);


    auto seg4d = writer<Seg4D>();
    seg4d.property("a", &Seg4D::a);
    seg4d.property("b", &Seg4D::b);

    auto seg4f = writer<Seg4F>();
    seg4f.property("a", &Seg4F::a);
    seg4f.property("b", &Seg4F::b);

    auto seg4i = writer<Seg4I>();
    seg4i.property("a", &Seg4I::a);
    seg4i.property("b", &Seg4I::b);

    auto seg4u = writer<Seg4U>();
    seg4u.property("a", &Seg4U::a);
    seg4u.property("b", &Seg4U::b);
    
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
