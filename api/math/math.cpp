////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "preamble.hpp"
#include "Fraction.hpp"

#include <basic/DelayInit.hpp>
#include <basic/meta/Init.hpp>

using namespace yq;

YQ_TYPE_IMPLEMENT(yq::Frac8)
YQ_TYPE_IMPLEMENT(yq::Frac16)
YQ_TYPE_IMPLEMENT(yq::Frac32)
YQ_TYPE_IMPLEMENT(yq::Frac64)

YQ_INVOKE(
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
);

namespace yq {
    void        initialize_math()
    {
    }
}
