////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

//  WARNING!  WARNING!
//
//  THIS FILE IS AUTO-GENERATED, CHANGES WILL BE CLOBBERED!

#include <math/preamble.hpp>

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

#include <math/shape/AxBox1.hpp>
#include <math/shape/AxBox2.hpp>
#include <math/shape/AxBox3.hpp>
#include <math/shape/AxBox4.hpp>


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

YQ_TYPE_IMPLEMENT(yq::Ten11D)
YQ_TYPE_IMPLEMENT(yq::Ten11F)
YQ_TYPE_IMPLEMENT(yq::Ten11I)
YQ_TYPE_IMPLEMENT(yq::Ten11U)

YQ_TYPE_IMPLEMENT(yq::Ten12D)
YQ_TYPE_IMPLEMENT(yq::Ten12F)
YQ_TYPE_IMPLEMENT(yq::Ten12I)
YQ_TYPE_IMPLEMENT(yq::Ten12U)

YQ_TYPE_IMPLEMENT(yq::Ten13D)
YQ_TYPE_IMPLEMENT(yq::Ten13F)
YQ_TYPE_IMPLEMENT(yq::Ten13I)
YQ_TYPE_IMPLEMENT(yq::Ten13U)

YQ_TYPE_IMPLEMENT(yq::Ten14D)
YQ_TYPE_IMPLEMENT(yq::Ten14F)
YQ_TYPE_IMPLEMENT(yq::Ten14I)
YQ_TYPE_IMPLEMENT(yq::Ten14U)

YQ_TYPE_IMPLEMENT(yq::Ten21D)
YQ_TYPE_IMPLEMENT(yq::Ten21F)
YQ_TYPE_IMPLEMENT(yq::Ten21I)
YQ_TYPE_IMPLEMENT(yq::Ten21U)

YQ_TYPE_IMPLEMENT(yq::Ten22D)
YQ_TYPE_IMPLEMENT(yq::Ten22F)
YQ_TYPE_IMPLEMENT(yq::Ten22I)
YQ_TYPE_IMPLEMENT(yq::Ten22U)

YQ_TYPE_IMPLEMENT(yq::Ten23D)
YQ_TYPE_IMPLEMENT(yq::Ten23F)
YQ_TYPE_IMPLEMENT(yq::Ten23I)
YQ_TYPE_IMPLEMENT(yq::Ten23U)

YQ_TYPE_IMPLEMENT(yq::Ten24D)
YQ_TYPE_IMPLEMENT(yq::Ten24F)
YQ_TYPE_IMPLEMENT(yq::Ten24I)
YQ_TYPE_IMPLEMENT(yq::Ten24U)

YQ_TYPE_IMPLEMENT(yq::Ten31D)
YQ_TYPE_IMPLEMENT(yq::Ten31F)
YQ_TYPE_IMPLEMENT(yq::Ten31I)
YQ_TYPE_IMPLEMENT(yq::Ten31U)

YQ_TYPE_IMPLEMENT(yq::Ten32D)
YQ_TYPE_IMPLEMENT(yq::Ten32F)
YQ_TYPE_IMPLEMENT(yq::Ten32I)
YQ_TYPE_IMPLEMENT(yq::Ten32U)

YQ_TYPE_IMPLEMENT(yq::Ten33D)
YQ_TYPE_IMPLEMENT(yq::Ten33F)
YQ_TYPE_IMPLEMENT(yq::Ten33I)
YQ_TYPE_IMPLEMENT(yq::Ten33U)

YQ_TYPE_IMPLEMENT(yq::Ten34D)
YQ_TYPE_IMPLEMENT(yq::Ten34F)
YQ_TYPE_IMPLEMENT(yq::Ten34I)
YQ_TYPE_IMPLEMENT(yq::Ten34U)

YQ_TYPE_IMPLEMENT(yq::Ten41D)
YQ_TYPE_IMPLEMENT(yq::Ten41F)
YQ_TYPE_IMPLEMENT(yq::Ten41I)
YQ_TYPE_IMPLEMENT(yq::Ten41U)

YQ_TYPE_IMPLEMENT(yq::Ten42D)
YQ_TYPE_IMPLEMENT(yq::Ten42F)
YQ_TYPE_IMPLEMENT(yq::Ten42I)
YQ_TYPE_IMPLEMENT(yq::Ten42U)

YQ_TYPE_IMPLEMENT(yq::Ten43D)
YQ_TYPE_IMPLEMENT(yq::Ten43F)
YQ_TYPE_IMPLEMENT(yq::Ten43I)
YQ_TYPE_IMPLEMENT(yq::Ten43U)

YQ_TYPE_IMPLEMENT(yq::Ten44D)
YQ_TYPE_IMPLEMENT(yq::Ten44F)
YQ_TYPE_IMPLEMENT(yq::Ten44I)
YQ_TYPE_IMPLEMENT(yq::Ten44U)

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
    auto ten11d = writer<Ten11D>();
    ten11d.property("xx", &Ten11D::xx);

    auto ten11f = writer<Ten11F>();
    ten11f.property("xx", &Ten11F::xx);

    auto ten11i = writer<Ten11I>();
    ten11i.property("xx", &Ten11I::xx);

    auto ten11u = writer<Ten11U>();
    ten11u.property("xx", &Ten11U::xx);


    auto ten12d = writer<Ten12D>();
    ten12d.property("xx", &Ten12D::xx);
    ten12d.property("xy", &Ten12D::xy);

    auto ten12f = writer<Ten12F>();
    ten12f.property("xx", &Ten12F::xx);
    ten12f.property("xy", &Ten12F::xy);

    auto ten12i = writer<Ten12I>();
    ten12i.property("xx", &Ten12I::xx);
    ten12i.property("xy", &Ten12I::xy);

    auto ten12u = writer<Ten12U>();
    ten12u.property("xx", &Ten12U::xx);
    ten12u.property("xy", &Ten12U::xy);


    auto ten13d = writer<Ten13D>();
    ten13d.property("xx", &Ten13D::xx);
    ten13d.property("xy", &Ten13D::xy);
    ten13d.property("xz", &Ten13D::xz);

    auto ten13f = writer<Ten13F>();
    ten13f.property("xx", &Ten13F::xx);
    ten13f.property("xy", &Ten13F::xy);
    ten13f.property("xz", &Ten13F::xz);

    auto ten13i = writer<Ten13I>();
    ten13i.property("xx", &Ten13I::xx);
    ten13i.property("xy", &Ten13I::xy);
    ten13i.property("xz", &Ten13I::xz);

    auto ten13u = writer<Ten13U>();
    ten13u.property("xx", &Ten13U::xx);
    ten13u.property("xy", &Ten13U::xy);
    ten13u.property("xz", &Ten13U::xz);


    auto ten14d = writer<Ten14D>();
    ten14d.property("xx", &Ten14D::xx);
    ten14d.property("xy", &Ten14D::xy);
    ten14d.property("xz", &Ten14D::xz);
    ten14d.property("xw", &Ten14D::xw);

    auto ten14f = writer<Ten14F>();
    ten14f.property("xx", &Ten14F::xx);
    ten14f.property("xy", &Ten14F::xy);
    ten14f.property("xz", &Ten14F::xz);
    ten14f.property("xw", &Ten14F::xw);

    auto ten14i = writer<Ten14I>();
    ten14i.property("xx", &Ten14I::xx);
    ten14i.property("xy", &Ten14I::xy);
    ten14i.property("xz", &Ten14I::xz);
    ten14i.property("xw", &Ten14I::xw);

    auto ten14u = writer<Ten14U>();
    ten14u.property("xx", &Ten14U::xx);
    ten14u.property("xy", &Ten14U::xy);
    ten14u.property("xz", &Ten14U::xz);
    ten14u.property("xw", &Ten14U::xw);


    auto ten21d = writer<Ten21D>();
    ten21d.property("xx", &Ten21D::xx);
    ten21d.property("yx", &Ten21D::yx);

    auto ten21f = writer<Ten21F>();
    ten21f.property("xx", &Ten21F::xx);
    ten21f.property("yx", &Ten21F::yx);

    auto ten21i = writer<Ten21I>();
    ten21i.property("xx", &Ten21I::xx);
    ten21i.property("yx", &Ten21I::yx);

    auto ten21u = writer<Ten21U>();
    ten21u.property("xx", &Ten21U::xx);
    ten21u.property("yx", &Ten21U::yx);


    auto ten22d = writer<Ten22D>();
    ten22d.property("xx", &Ten22D::xx);
    ten22d.property("xy", &Ten22D::xy);
    ten22d.property("yx", &Ten22D::yx);
    ten22d.property("yy", &Ten22D::yy);

    auto ten22f = writer<Ten22F>();
    ten22f.property("xx", &Ten22F::xx);
    ten22f.property("xy", &Ten22F::xy);
    ten22f.property("yx", &Ten22F::yx);
    ten22f.property("yy", &Ten22F::yy);

    auto ten22i = writer<Ten22I>();
    ten22i.property("xx", &Ten22I::xx);
    ten22i.property("xy", &Ten22I::xy);
    ten22i.property("yx", &Ten22I::yx);
    ten22i.property("yy", &Ten22I::yy);

    auto ten22u = writer<Ten22U>();
    ten22u.property("xx", &Ten22U::xx);
    ten22u.property("xy", &Ten22U::xy);
    ten22u.property("yx", &Ten22U::yx);
    ten22u.property("yy", &Ten22U::yy);


    auto ten23d = writer<Ten23D>();
    ten23d.property("xx", &Ten23D::xx);
    ten23d.property("xy", &Ten23D::xy);
    ten23d.property("xz", &Ten23D::xz);
    ten23d.property("yx", &Ten23D::yx);
    ten23d.property("yy", &Ten23D::yy);
    ten23d.property("yz", &Ten23D::yz);

    auto ten23f = writer<Ten23F>();
    ten23f.property("xx", &Ten23F::xx);
    ten23f.property("xy", &Ten23F::xy);
    ten23f.property("xz", &Ten23F::xz);
    ten23f.property("yx", &Ten23F::yx);
    ten23f.property("yy", &Ten23F::yy);
    ten23f.property("yz", &Ten23F::yz);

    auto ten23i = writer<Ten23I>();
    ten23i.property("xx", &Ten23I::xx);
    ten23i.property("xy", &Ten23I::xy);
    ten23i.property("xz", &Ten23I::xz);
    ten23i.property("yx", &Ten23I::yx);
    ten23i.property("yy", &Ten23I::yy);
    ten23i.property("yz", &Ten23I::yz);

    auto ten23u = writer<Ten23U>();
    ten23u.property("xx", &Ten23U::xx);
    ten23u.property("xy", &Ten23U::xy);
    ten23u.property("xz", &Ten23U::xz);
    ten23u.property("yx", &Ten23U::yx);
    ten23u.property("yy", &Ten23U::yy);
    ten23u.property("yz", &Ten23U::yz);


    auto ten24d = writer<Ten24D>();
    ten24d.property("xx", &Ten24D::xx);
    ten24d.property("xy", &Ten24D::xy);
    ten24d.property("xz", &Ten24D::xz);
    ten24d.property("xw", &Ten24D::xw);
    ten24d.property("yx", &Ten24D::yx);
    ten24d.property("yy", &Ten24D::yy);
    ten24d.property("yz", &Ten24D::yz);
    ten24d.property("yw", &Ten24D::yw);

    auto ten24f = writer<Ten24F>();
    ten24f.property("xx", &Ten24F::xx);
    ten24f.property("xy", &Ten24F::xy);
    ten24f.property("xz", &Ten24F::xz);
    ten24f.property("xw", &Ten24F::xw);
    ten24f.property("yx", &Ten24F::yx);
    ten24f.property("yy", &Ten24F::yy);
    ten24f.property("yz", &Ten24F::yz);
    ten24f.property("yw", &Ten24F::yw);

    auto ten24i = writer<Ten24I>();
    ten24i.property("xx", &Ten24I::xx);
    ten24i.property("xy", &Ten24I::xy);
    ten24i.property("xz", &Ten24I::xz);
    ten24i.property("xw", &Ten24I::xw);
    ten24i.property("yx", &Ten24I::yx);
    ten24i.property("yy", &Ten24I::yy);
    ten24i.property("yz", &Ten24I::yz);
    ten24i.property("yw", &Ten24I::yw);

    auto ten24u = writer<Ten24U>();
    ten24u.property("xx", &Ten24U::xx);
    ten24u.property("xy", &Ten24U::xy);
    ten24u.property("xz", &Ten24U::xz);
    ten24u.property("xw", &Ten24U::xw);
    ten24u.property("yx", &Ten24U::yx);
    ten24u.property("yy", &Ten24U::yy);
    ten24u.property("yz", &Ten24U::yz);
    ten24u.property("yw", &Ten24U::yw);


    auto ten31d = writer<Ten31D>();
    ten31d.property("xx", &Ten31D::xx);
    ten31d.property("yx", &Ten31D::yx);
    ten31d.property("zx", &Ten31D::zx);

    auto ten31f = writer<Ten31F>();
    ten31f.property("xx", &Ten31F::xx);
    ten31f.property("yx", &Ten31F::yx);
    ten31f.property("zx", &Ten31F::zx);

    auto ten31i = writer<Ten31I>();
    ten31i.property("xx", &Ten31I::xx);
    ten31i.property("yx", &Ten31I::yx);
    ten31i.property("zx", &Ten31I::zx);

    auto ten31u = writer<Ten31U>();
    ten31u.property("xx", &Ten31U::xx);
    ten31u.property("yx", &Ten31U::yx);
    ten31u.property("zx", &Ten31U::zx);


    auto ten32d = writer<Ten32D>();
    ten32d.property("xx", &Ten32D::xx);
    ten32d.property("xy", &Ten32D::xy);
    ten32d.property("yx", &Ten32D::yx);
    ten32d.property("yy", &Ten32D::yy);
    ten32d.property("zx", &Ten32D::zx);
    ten32d.property("zy", &Ten32D::zy);

    auto ten32f = writer<Ten32F>();
    ten32f.property("xx", &Ten32F::xx);
    ten32f.property("xy", &Ten32F::xy);
    ten32f.property("yx", &Ten32F::yx);
    ten32f.property("yy", &Ten32F::yy);
    ten32f.property("zx", &Ten32F::zx);
    ten32f.property("zy", &Ten32F::zy);

    auto ten32i = writer<Ten32I>();
    ten32i.property("xx", &Ten32I::xx);
    ten32i.property("xy", &Ten32I::xy);
    ten32i.property("yx", &Ten32I::yx);
    ten32i.property("yy", &Ten32I::yy);
    ten32i.property("zx", &Ten32I::zx);
    ten32i.property("zy", &Ten32I::zy);

    auto ten32u = writer<Ten32U>();
    ten32u.property("xx", &Ten32U::xx);
    ten32u.property("xy", &Ten32U::xy);
    ten32u.property("yx", &Ten32U::yx);
    ten32u.property("yy", &Ten32U::yy);
    ten32u.property("zx", &Ten32U::zx);
    ten32u.property("zy", &Ten32U::zy);


    auto ten33d = writer<Ten33D>();
    ten33d.property("xx", &Ten33D::xx);
    ten33d.property("xy", &Ten33D::xy);
    ten33d.property("xz", &Ten33D::xz);
    ten33d.property("yx", &Ten33D::yx);
    ten33d.property("yy", &Ten33D::yy);
    ten33d.property("yz", &Ten33D::yz);
    ten33d.property("zx", &Ten33D::zx);
    ten33d.property("zy", &Ten33D::zy);
    ten33d.property("zz", &Ten33D::zz);

    auto ten33f = writer<Ten33F>();
    ten33f.property("xx", &Ten33F::xx);
    ten33f.property("xy", &Ten33F::xy);
    ten33f.property("xz", &Ten33F::xz);
    ten33f.property("yx", &Ten33F::yx);
    ten33f.property("yy", &Ten33F::yy);
    ten33f.property("yz", &Ten33F::yz);
    ten33f.property("zx", &Ten33F::zx);
    ten33f.property("zy", &Ten33F::zy);
    ten33f.property("zz", &Ten33F::zz);

    auto ten33i = writer<Ten33I>();
    ten33i.property("xx", &Ten33I::xx);
    ten33i.property("xy", &Ten33I::xy);
    ten33i.property("xz", &Ten33I::xz);
    ten33i.property("yx", &Ten33I::yx);
    ten33i.property("yy", &Ten33I::yy);
    ten33i.property("yz", &Ten33I::yz);
    ten33i.property("zx", &Ten33I::zx);
    ten33i.property("zy", &Ten33I::zy);
    ten33i.property("zz", &Ten33I::zz);

    auto ten33u = writer<Ten33U>();
    ten33u.property("xx", &Ten33U::xx);
    ten33u.property("xy", &Ten33U::xy);
    ten33u.property("xz", &Ten33U::xz);
    ten33u.property("yx", &Ten33U::yx);
    ten33u.property("yy", &Ten33U::yy);
    ten33u.property("yz", &Ten33U::yz);
    ten33u.property("zx", &Ten33U::zx);
    ten33u.property("zy", &Ten33U::zy);
    ten33u.property("zz", &Ten33U::zz);


    auto ten34d = writer<Ten34D>();
    ten34d.property("xx", &Ten34D::xx);
    ten34d.property("xy", &Ten34D::xy);
    ten34d.property("xz", &Ten34D::xz);
    ten34d.property("xw", &Ten34D::xw);
    ten34d.property("yx", &Ten34D::yx);
    ten34d.property("yy", &Ten34D::yy);
    ten34d.property("yz", &Ten34D::yz);
    ten34d.property("yw", &Ten34D::yw);
    ten34d.property("zx", &Ten34D::zx);
    ten34d.property("zy", &Ten34D::zy);
    ten34d.property("zz", &Ten34D::zz);
    ten34d.property("zw", &Ten34D::zw);

    auto ten34f = writer<Ten34F>();
    ten34f.property("xx", &Ten34F::xx);
    ten34f.property("xy", &Ten34F::xy);
    ten34f.property("xz", &Ten34F::xz);
    ten34f.property("xw", &Ten34F::xw);
    ten34f.property("yx", &Ten34F::yx);
    ten34f.property("yy", &Ten34F::yy);
    ten34f.property("yz", &Ten34F::yz);
    ten34f.property("yw", &Ten34F::yw);
    ten34f.property("zx", &Ten34F::zx);
    ten34f.property("zy", &Ten34F::zy);
    ten34f.property("zz", &Ten34F::zz);
    ten34f.property("zw", &Ten34F::zw);

    auto ten34i = writer<Ten34I>();
    ten34i.property("xx", &Ten34I::xx);
    ten34i.property("xy", &Ten34I::xy);
    ten34i.property("xz", &Ten34I::xz);
    ten34i.property("xw", &Ten34I::xw);
    ten34i.property("yx", &Ten34I::yx);
    ten34i.property("yy", &Ten34I::yy);
    ten34i.property("yz", &Ten34I::yz);
    ten34i.property("yw", &Ten34I::yw);
    ten34i.property("zx", &Ten34I::zx);
    ten34i.property("zy", &Ten34I::zy);
    ten34i.property("zz", &Ten34I::zz);
    ten34i.property("zw", &Ten34I::zw);

    auto ten34u = writer<Ten34U>();
    ten34u.property("xx", &Ten34U::xx);
    ten34u.property("xy", &Ten34U::xy);
    ten34u.property("xz", &Ten34U::xz);
    ten34u.property("xw", &Ten34U::xw);
    ten34u.property("yx", &Ten34U::yx);
    ten34u.property("yy", &Ten34U::yy);
    ten34u.property("yz", &Ten34U::yz);
    ten34u.property("yw", &Ten34U::yw);
    ten34u.property("zx", &Ten34U::zx);
    ten34u.property("zy", &Ten34U::zy);
    ten34u.property("zz", &Ten34U::zz);
    ten34u.property("zw", &Ten34U::zw);


    auto ten41d = writer<Ten41D>();
    ten41d.property("xx", &Ten41D::xx);
    ten41d.property("yx", &Ten41D::yx);
    ten41d.property("zx", &Ten41D::zx);
    ten41d.property("wx", &Ten41D::wx);

    auto ten41f = writer<Ten41F>();
    ten41f.property("xx", &Ten41F::xx);
    ten41f.property("yx", &Ten41F::yx);
    ten41f.property("zx", &Ten41F::zx);
    ten41f.property("wx", &Ten41F::wx);

    auto ten41i = writer<Ten41I>();
    ten41i.property("xx", &Ten41I::xx);
    ten41i.property("yx", &Ten41I::yx);
    ten41i.property("zx", &Ten41I::zx);
    ten41i.property("wx", &Ten41I::wx);

    auto ten41u = writer<Ten41U>();
    ten41u.property("xx", &Ten41U::xx);
    ten41u.property("yx", &Ten41U::yx);
    ten41u.property("zx", &Ten41U::zx);
    ten41u.property("wx", &Ten41U::wx);


    auto ten42d = writer<Ten42D>();
    ten42d.property("xx", &Ten42D::xx);
    ten42d.property("xy", &Ten42D::xy);
    ten42d.property("yx", &Ten42D::yx);
    ten42d.property("yy", &Ten42D::yy);
    ten42d.property("zx", &Ten42D::zx);
    ten42d.property("zy", &Ten42D::zy);
    ten42d.property("wx", &Ten42D::wx);
    ten42d.property("wy", &Ten42D::wy);

    auto ten42f = writer<Ten42F>();
    ten42f.property("xx", &Ten42F::xx);
    ten42f.property("xy", &Ten42F::xy);
    ten42f.property("yx", &Ten42F::yx);
    ten42f.property("yy", &Ten42F::yy);
    ten42f.property("zx", &Ten42F::zx);
    ten42f.property("zy", &Ten42F::zy);
    ten42f.property("wx", &Ten42F::wx);
    ten42f.property("wy", &Ten42F::wy);

    auto ten42i = writer<Ten42I>();
    ten42i.property("xx", &Ten42I::xx);
    ten42i.property("xy", &Ten42I::xy);
    ten42i.property("yx", &Ten42I::yx);
    ten42i.property("yy", &Ten42I::yy);
    ten42i.property("zx", &Ten42I::zx);
    ten42i.property("zy", &Ten42I::zy);
    ten42i.property("wx", &Ten42I::wx);
    ten42i.property("wy", &Ten42I::wy);

    auto ten42u = writer<Ten42U>();
    ten42u.property("xx", &Ten42U::xx);
    ten42u.property("xy", &Ten42U::xy);
    ten42u.property("yx", &Ten42U::yx);
    ten42u.property("yy", &Ten42U::yy);
    ten42u.property("zx", &Ten42U::zx);
    ten42u.property("zy", &Ten42U::zy);
    ten42u.property("wx", &Ten42U::wx);
    ten42u.property("wy", &Ten42U::wy);


    auto ten43d = writer<Ten43D>();
    ten43d.property("xx", &Ten43D::xx);
    ten43d.property("xy", &Ten43D::xy);
    ten43d.property("xz", &Ten43D::xz);
    ten43d.property("yx", &Ten43D::yx);
    ten43d.property("yy", &Ten43D::yy);
    ten43d.property("yz", &Ten43D::yz);
    ten43d.property("zx", &Ten43D::zx);
    ten43d.property("zy", &Ten43D::zy);
    ten43d.property("zz", &Ten43D::zz);
    ten43d.property("wx", &Ten43D::wx);
    ten43d.property("wy", &Ten43D::wy);
    ten43d.property("wz", &Ten43D::wz);

    auto ten43f = writer<Ten43F>();
    ten43f.property("xx", &Ten43F::xx);
    ten43f.property("xy", &Ten43F::xy);
    ten43f.property("xz", &Ten43F::xz);
    ten43f.property("yx", &Ten43F::yx);
    ten43f.property("yy", &Ten43F::yy);
    ten43f.property("yz", &Ten43F::yz);
    ten43f.property("zx", &Ten43F::zx);
    ten43f.property("zy", &Ten43F::zy);
    ten43f.property("zz", &Ten43F::zz);
    ten43f.property("wx", &Ten43F::wx);
    ten43f.property("wy", &Ten43F::wy);
    ten43f.property("wz", &Ten43F::wz);

    auto ten43i = writer<Ten43I>();
    ten43i.property("xx", &Ten43I::xx);
    ten43i.property("xy", &Ten43I::xy);
    ten43i.property("xz", &Ten43I::xz);
    ten43i.property("yx", &Ten43I::yx);
    ten43i.property("yy", &Ten43I::yy);
    ten43i.property("yz", &Ten43I::yz);
    ten43i.property("zx", &Ten43I::zx);
    ten43i.property("zy", &Ten43I::zy);
    ten43i.property("zz", &Ten43I::zz);
    ten43i.property("wx", &Ten43I::wx);
    ten43i.property("wy", &Ten43I::wy);
    ten43i.property("wz", &Ten43I::wz);

    auto ten43u = writer<Ten43U>();
    ten43u.property("xx", &Ten43U::xx);
    ten43u.property("xy", &Ten43U::xy);
    ten43u.property("xz", &Ten43U::xz);
    ten43u.property("yx", &Ten43U::yx);
    ten43u.property("yy", &Ten43U::yy);
    ten43u.property("yz", &Ten43U::yz);
    ten43u.property("zx", &Ten43U::zx);
    ten43u.property("zy", &Ten43U::zy);
    ten43u.property("zz", &Ten43U::zz);
    ten43u.property("wx", &Ten43U::wx);
    ten43u.property("wy", &Ten43U::wy);
    ten43u.property("wz", &Ten43U::wz);


    auto ten44d = writer<Ten44D>();
    ten44d.property("xx", &Ten44D::xx);
    ten44d.property("xy", &Ten44D::xy);
    ten44d.property("xz", &Ten44D::xz);
    ten44d.property("xw", &Ten44D::xw);
    ten44d.property("yx", &Ten44D::yx);
    ten44d.property("yy", &Ten44D::yy);
    ten44d.property("yz", &Ten44D::yz);
    ten44d.property("yw", &Ten44D::yw);
    ten44d.property("zx", &Ten44D::zx);
    ten44d.property("zy", &Ten44D::zy);
    ten44d.property("zz", &Ten44D::zz);
    ten44d.property("zw", &Ten44D::zw);
    ten44d.property("wx", &Ten44D::wx);
    ten44d.property("wy", &Ten44D::wy);
    ten44d.property("wz", &Ten44D::wz);
    ten44d.property("ww", &Ten44D::ww);

    auto ten44f = writer<Ten44F>();
    ten44f.property("xx", &Ten44F::xx);
    ten44f.property("xy", &Ten44F::xy);
    ten44f.property("xz", &Ten44F::xz);
    ten44f.property("xw", &Ten44F::xw);
    ten44f.property("yx", &Ten44F::yx);
    ten44f.property("yy", &Ten44F::yy);
    ten44f.property("yz", &Ten44F::yz);
    ten44f.property("yw", &Ten44F::yw);
    ten44f.property("zx", &Ten44F::zx);
    ten44f.property("zy", &Ten44F::zy);
    ten44f.property("zz", &Ten44F::zz);
    ten44f.property("zw", &Ten44F::zw);
    ten44f.property("wx", &Ten44F::wx);
    ten44f.property("wy", &Ten44F::wy);
    ten44f.property("wz", &Ten44F::wz);
    ten44f.property("ww", &Ten44F::ww);

    auto ten44i = writer<Ten44I>();
    ten44i.property("xx", &Ten44I::xx);
    ten44i.property("xy", &Ten44I::xy);
    ten44i.property("xz", &Ten44I::xz);
    ten44i.property("xw", &Ten44I::xw);
    ten44i.property("yx", &Ten44I::yx);
    ten44i.property("yy", &Ten44I::yy);
    ten44i.property("yz", &Ten44I::yz);
    ten44i.property("yw", &Ten44I::yw);
    ten44i.property("zx", &Ten44I::zx);
    ten44i.property("zy", &Ten44I::zy);
    ten44i.property("zz", &Ten44I::zz);
    ten44i.property("zw", &Ten44I::zw);
    ten44i.property("wx", &Ten44I::wx);
    ten44i.property("wy", &Ten44I::wy);
    ten44i.property("wz", &Ten44I::wz);
    ten44i.property("ww", &Ten44I::ww);

    auto ten44u = writer<Ten44U>();
    ten44u.property("xx", &Ten44U::xx);
    ten44u.property("xy", &Ten44U::xy);
    ten44u.property("xz", &Ten44U::xz);
    ten44u.property("xw", &Ten44U::xw);
    ten44u.property("yx", &Ten44U::yx);
    ten44u.property("yy", &Ten44U::yy);
    ten44u.property("yz", &Ten44U::yz);
    ten44u.property("yw", &Ten44U::yw);
    ten44u.property("zx", &Ten44U::zx);
    ten44u.property("zy", &Ten44U::zy);
    ten44u.property("zz", &Ten44U::zz);
    ten44u.property("zw", &Ten44U::zw);
    ten44u.property("wx", &Ten44U::wx);
    ten44u.property("wy", &Ten44U::wy);
    ten44u.property("wz", &Ten44U::wz);
    ten44u.property("ww", &Ten44U::ww);


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


)
