////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/preamble.hpp>

#include <basic/StreamOps.hpp>
#include <basic/meta/InfoBinder.hpp>
#include <basic/trait/always_false.hpp>

#include <math/trait/has_abs.hpp>
#include <math/trait/has_copysign.hpp>
#include <math/trait/has_identity.hpp>
#include <math/trait/has_is_finite.hpp>
#include <math/trait/has_nan.hpp>
#include <math/trait/has_one.hpp>
#include <math/trait/has_sqrt.hpp>
#include <math/trait/has_zero.hpp>
#include <math/trait/ieee754.hpp>

#include <math/dims.hpp>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <numbers>
#include <utility>

#include <log4cpp/CategoryStream.hh>

#include <glm/glm.hpp>

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //      MACROS
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#ifdef PI
#undef PI
#endif

/*! \brief Dot product operator

    As C++ doesn't let us define new operators, we must make do with the
    existing operators.  
    
    This is the macro used for the dot product between zvecs.
    
    \note   It is HIGHLY recommended to use parathensis as the operational 
        priorities are based on C++'s interpretation of the actual symbol,
        which is not necessarily our intended order.
*/
#define DOT         |

/*! \brief Inner product operator

    As C++ doesn't let us define new operators, we must make do with the
    existing operators.  
    
    This is the macro used for the inner product.

    \note   It is HIGHLY recommended to use parathensis as the operational 
        priorities are based on C++'s interpretation of the actual symbol,
        which is not necessarily our intended order.
*/
#define INNER       &
#define INNER_EQ    &=

/*! \brief Outer product operator

    As C++ doesn't let us define new operators, we must make do with the
    existing operators.  
    
    This is the macro used for the outer product.

    \note   It is HIGHLY recommended to use parathensis as the operational 
        priorities are based on C++'s interpretation of the actual symbol,
        which is not necessarily our intended order.
*/
#define OUTER       ^
#define OUTER_EQ    ^=

/*! \brief Outer Vector to Tensor operator

    As C++ doesn't let us define new operators, we must make do with the
    existing operators.  
    
    This is the macro used for the outer zvec product between zvecs.
    This is different than the general outer product because this one
    produces ztens, whereas the general outer product makes for zbvecs.
    
    Also, this operator can produce non-square ztens (ie, 3x2).

    \note   It is HIGHLY recommended to use parathensis as the operational 
        priorities are based on C++'s interpretation of the actual symbol,
        which is not necessarily our intended order.
*/
#define OTIMES      &&

/*! \brief Cross product operator

    As C++ doesn't let us define new operators, we must make do with the
    existing operators.  
    
    This is the macro used for the cross product between zvecs.

    \note   It is HIGHLY recommended to use parathensis as the operational 
        priorities are based on C++'s interpretation of the actual symbol,
        which is not necessarily our intended order.
*/
#define CROSS       %
#define CROSS_EQ   %=

namespace std {
    template <typename> class complex;
}


namespace yq {
    

    static constexpr const double   pi      = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862;
    static constexpr const double   sqrt2   = 1.4142135623730950488016887242096980785696718753769480731766797379907324784621;
    static constexpr const double   sqrt3   = 1.7320508075688772935274463415058723669428052538103806280558069794519330169088;
    static constexpr const int64_t  max64   = std::numeric_limits<int64_t>::max();
    static constexpr const uint64_t maxu64  = std::numeric_limits<uint64_t>::max();

    static constexpr double   deg2rad = pi / 180.0;
    static constexpr double   rad2deg = 180.0 / pi;

    template <typename F, typename S>
    S       greatest(const std::pair<F,S>& a, const std::pair<F,S>& b)
    {
        if(a.first >= b.first)
            return a.second;
        else
            return b.second;
    }

    template <typename F, typename S>
    S       greatest(const std::pair<F,S>& a, const std::pair<F,S>& b, const std::pair<F,S>& c)
    {
        if(c.first >= std::max(a.first, b.first))
            return c.second;
        else if(b.first >= std::max(a.first, c.first))
            return b.second;
        else
            return a.second;
    }


    inline int   iround(double v)
    {
        return (int)(std::round(v)+std::copysign(0.1,v));
    }

    inline int   iceil(double r)
    {
        return (int)(ceil(r)+std::copysign(0.1,r));
    }

    inline int   itrunc(double r)
    {
        return (int)(trunc(r)+std::copysign(0.1,r));
    }

    inline int   ifloor(double r)
    {
        return (int)(floor(r)+std::copysign(0.1,r));
    }

    template <typename> struct AxBox1;
    template <typename> struct AxBox2;
    template <typename> struct AxBox3;
    template <typename> struct AxBox4;
    template <typename> struct Circle2;
    template <typename> struct Coord2;
    template <typename> struct Fraction;
    template <typename> struct Normal2;
    template <typename> struct Normal3;
    template <typename> struct Plane3;
    template <typename> struct Quaternion3;
    template <typename> struct Ray2;
    template <typename> struct Ray3;
    template <typename> struct Ray4;
    template <typename> struct Rectangle2;
    template <typename> struct RGB;
    template <typename> struct RGBA;
    template <typename> struct Segment1;
    template <typename> struct Segment2;
    template <typename> struct Segment3;
    template <typename> struct Segment4;
    template <typename> struct Size2;
    template <typename> struct Sphere3;
    template <typename> struct Sphere4;
    template <typename> struct Tensor11;
    template <typename> struct Tensor12;
    template <typename> struct Tensor13;
    template <typename> struct Tensor14;
    template <typename> struct Tensor21;
    template <typename> struct Tensor22;
    template <typename> struct Tensor23;
    template <typename> struct Tensor24;
    template <typename> struct Tensor31;
    template <typename> struct Tensor32;
    template <typename> struct Tensor33;
    template <typename> struct Tensor34;
    template <typename> struct Tensor41;
    template <typename> struct Tensor42;
    template <typename> struct Tensor43;
    template <typename> struct Tensor44;
    template <typename> struct Tetrahedron3;
    template <typename> struct Triangle2;
    template <typename> struct Triangle3;
    template <typename> struct Triangle4;
    template <typename> struct Vector1;
    template <typename> struct Vector2;
    template <typename> struct Vector3;
    template <typename> struct Vector4;

    //  ------------------------------------------------
    //  COMPLEX NUMBERS


    using ComplexD                  = std::complex<double>;
    using ComplexF                  = std::complex<float>;
    using ComplexI                  = std::complex<int>;
    using ComplexU                  = std::complex<unsigned>;

    
    //  ------------------------------------------------
    //  FORWARD DECLARE UNITS

    template <typename T, typename DIM>             struct MKS;
    template <typename T, typename DIM, double K>   struct SCALED;

    //  Unit sizes double checked with wikipedia!
    namespace unit {
        using Acre                      = SCALED<double,dim::Area,4046.86>;
        using Ampere                    = MKS<double,dim::Current>;
        using Arcminute                 = SCALED<double,dim::Angle,deg2rad/60.>;
        using Arcsecond                 = SCALED<double,dim::Angle,deg2rad/3600.>;
        using Are                       = SCALED<double,dim::Area,100.0>;
        using Ångström                  = SCALED<double,dim::Length,1e-10>;
        using AstronomicalUnit          = SCALED<double,dim::Length,149597870700.0>;
        using Bar                       = SCALED<double,dim::Pressure,1e5>;
        using Barn                      = SCALED<double,dim::Area,1e-28>;
        using Barye                     = SCALED<double,dim::Pressure,0.1>;
        using BritishThermalUnit        = SCALED<double,dim::Energy,1055.05585262>; // the definition is ambiguous, choosing
        using Calorie                   = SCALED<double,dim::Energy,4.184>;
        using Candela                   = MKS<double,dim::Intensity>;
        using Centimeter                = SCALED<double,dim::Length,0.01>;
        using Centimeter²               = SCALED<double,dim::Area,1e-4>;
        using Centimeter³               = SCALED<double,dim::Volume,1e-6>;
        using CentimeterPerSecond       = SCALED<double,dim::Speed,1e-2>;
        using CentimeterPerSecond²      = SCALED<double,dim::Acceleration,1e-2>;
        using Dalton                    = SCALED<double,dim::Mass,1.66005390666050e-27>; //!< Atomic Mass Unit
        using Day                       = SCALED<double,dim::Time,86400.0>;
        using Decibar                   = SCALED<double,dim::Pressure,1e4>;
        using Degree                    = SCALED<double,dim::Angle,deg2rad>;
        using Degree²                   = SCALED<double,dim::Angle,deg2rad*deg2rad>;
        using DegreeF                   = SCALED<float,dim::Angle,deg2rad>;
        using DegreePerMinute           = SCALED<double,dim::AngularVelocity,deg2rad/60.>;
        using DegreePerSecond           = SCALED<double,dim::AngularVelocity,deg2rad>;
        using DegreePerSecond²          = SCALED<double,dim::AngularAcceleration,deg2rad>;
        using Dioptre                   = MKS<double,dim::PerLength>;
        using Dyne                      = SCALED<double,dim::Force,1e-5>;
        using ElectronVolt              = SCALED<double,dim::Energy,1.602176634-19>;    // per 2019 definition
        using Erg                       = SCALED<double,dim::Energy,1e-7>; // used in CGS units
        using ErgPerSecond              = SCALED<double,dim::Power,1e-7>; // used in CGS units
        using Fathom                    = SCALED<double,dim::Length,1.8288>;
        using Foot                      = SCALED<double,dim::Length,0.3048>;
        using Foot²                     = SCALED<double,dim::Length,9.290304e-2>;
        using Foot³                     = SCALED<double,dim::Length,2.8316846592e-2>;
        using FootF                     = SCALED<float,dim::Length,0.3048>;
        using FootPerSecond             = SCALED<double,dim::Speed,0.3048>;
        using FootPerSecond²            = SCALED<double,dim::Acceleration,0.3048>;
        using Fortnight                 = SCALED<double,dim::Time, 1209600.0>;
        using Furlong                   = SCALED<double,dim::Length,201.168>;
        using FurlongPerFortnight       = SCALED<double,dim::Length,201.168/1209600.0>;
        using Gallon                    = SCALED<double,dim::Volume,3.785411784>;
        using GallonPerMinute           = SCALED<double,dim::VolumeFlowRate,3.785411784/60.0>;
        using GigaElectronVolt          = SCALED<double,dim::Energy,1.602176634-10>;
        using Gigahertz                 = SCALED<double,dim::Frequency,1e9>;
        using Gigajoule                 = SCALED<double,dim::Energy,1e9>;
        using Gigapascal                = SCALED<double,dim::Pressure,1e9>;
        using Gigawatt                  = SCALED<double,dim::Power,1e9>;
        using Gradian                   = SCALED<double,dim::Angle,std::numbers::pi_v<double>/200.0>;
        using Gram                      = SCALED<double,dim::Mass,1e-3>;
        using GramPerSecond             = SCALED<double,dim::MassFlow,1e-3>;
        using Gravity                   = SCALED<double,dim::Acceleration,9.80665>;
        using Hectare                   = SCALED<double,dim::Area,1e4>;
        using Hertz                     = MKS<double,dim::Frequency>;
        using HertzPerSecond            = MKS<double,dim::FrequencyRate>;
        using Horsepower                = SCALED<double,dim::Power,745.69987>;  // using mechanical horsepower
        using Hour                      = SCALED<double,dim::Time, 3600.0>;
        using Inch                      = SCALED<double,dim::Length,0.0254>;
        using Inch²                     = SCALED<double,dim::Area,6.4516e-4>;
        using Inch³                     = SCALED<double,dim::Volume,1.6387064e-5>;
        using Joule                     = MKS<double,dim::Energy>;
        using JoulePerKelvin            = MKS<double,dim::Heat>;
        using JoulePerKilogram          = MKS<double,dim::SpecificEnergy>;
        using JoulePerKilogramKelvin    = MKS<double,dim::SpecificMolecularHeat>;
        using JoulePerMole              = MKS<double,dim::SpecificMolecularEnergy>;
        using JoulePerMeter³            = MKS<double,dim::Pressure>;
        using Kelvin                    = MKS<double,dim::Temperature>;
        using Kiloampere                = SCALED<double,dim::Current,1000.>;
        using Kilocalorie               = SCALED<double,dim::Energy,4.184e3>;
        using KiloElectronVolt          = SCALED<double,dim::Energy,1.602176634-16>;
        using Kilogram                  = MKS<double,dim::Mass>;
        using KilogramMeterPerSecond    = MKS<double,dim::Impulse>; //!< Alias to N*s
        using KilogramPerSecond         = MKS<double,dim::MassFlow>;
        using KilogramPerSecond²        = MKS<double,dim::MassFlowRate>;
        using Kilohertz                 = SCALED<double,dim::Frequency,1e3>;
        using Kilojoule                 = SCALED<double,dim::Energy,1e3>;
        using Kilolitre                 = MKS<double,dim::Volume>;
        using Kilometer                 = SCALED<double,dim::Length,1000.0>;
        using Kilometer²                = SCALED<double,dim::Area,1e6>;
        using Kilometer³                = SCALED<double,dim::Volume,1e9>;
        using KilometerPerHour          = SCALED<double,dim::Speed,3600.0/1000.0>;
        using KilometerPerSecond        = SCALED<double,dim::Speed,1e3>;
        using KilometerPerSecond²       = SCALED<double,dim::Acceleration,1e3>;
        using Kilomole                  = SCALED<double,dim::Mass,1e3>;
        using Kilonewton                = SCALED<double,dim::Force,1e3>;
        using Kilopascal                = SCALED<double,dim::Pressure,1e3>;
        using Kilowatt                  = SCALED<double,dim::Power,1e3>;
        using KilowattHour              = SCALED<double,dim::Energy,1e3*3600.>;
        using Knot                      = SCALED<double,dim::Speed,1852./3600.>;
        using LightHour                 = SCALED<double,dim::Length,1079252848800.0>;
        using LightMinute               = SCALED<double,dim::Length,17987547480.0>;
        using LightSecond               = SCALED<double,dim::Length,299792458.0>;
        using LightSpeed                = SCALED<double,dim::Speed,299792458.0>;
        using LightYear                 = SCALED<double,dim::Length, 9460730472580800.0>;
        using Litre                     = SCALED<double,dim::Volume, 1e-3>;
        using LongTon                   = SCALED<double,dim::Mass,1016.0469088>;
        using Megaampere                = SCALED<double,dim::Current,1e6>;
        using MegaElectronVolt          = SCALED<double,dim::Energy,1.602176634-13>;
        using Megahertz                 = SCALED<double,dim::Frequency,1e6>;
        using Megajoule                 = SCALED<double,dim::Energy,1e6>;
        using Meganewton                = SCALED<double,dim::Force,1e6>;
        using Megapascal                = SCALED<double,dim::Pressure,1e6>;
        using Megawatt                  = SCALED<double,dim::Power,1e6>;
        using Meter                     = MKS<double,dim::Length>;
        using Meter²                    = MKS<double,dim::Area>;
        using Meter²PerSecond           = MKS<double,dim::AreaFlow>;
        using Meter²PerSecond²          = MKS<double,dim::SpecificEnergy>;
        using Meter³                    = MKS<double,dim::Volume>;
        using Meter³PerSecond           = MKS<double,dim::VolumeFlow>;
        using Meter³PerSecond²          = MKS<double,dim::VolumeFlowRate>;
        using MeterF                    = MKS<float,dim::Length>;
        using MeterPerSecond            = MKS<double,dim::Speed>;
        using MeterPerSecond²           = MKS<double,dim::Acceleration>;
        using Microampere               = SCALED<double,dim::Current,1e-6>;
        using MicroArcsecond            = SCALED<double,dim::Angle,deg2rad/3600e6>;
        using Microgram                 = SCALED<double,dim::Mass,1e-9>;
        using MicrogramPerSecond        = SCALED<double,dim::MassFlow,1e-9>;
        using Microjoule                = SCALED<double,dim::Energy,1e-6>;
        using Microlitre                = SCALED<double,dim::Volume, 1e-12>;
        using Micrometer                = SCALED<double,dim::Length,1e-6>;
        using MicrometerPerSecond       = SCALED<double,dim::Speed,1e-6>;
        using MicrometerPerSecond²      = SCALED<double,dim::Acceleration,1e-6>;
        using Micronewton               = SCALED<double,dim::Force,1e-6>;
        using Micropascal               = SCALED<double,dim::Pressure,1e-6>;
        using Microwatt                 = SCALED<double,dim::Power,1e-6>;
        using Mile                      = SCALED<double,dim::Length,1609.344>;
        using MilePerHour               = SCALED<double,dim::Speed,1609.344/3600.>;
        using Milliampere               = SCALED<double,dim::Current,1e-3>;
        using MilliArcsecond            = SCALED<double,dim::Angle,deg2rad/3600e3>;
        using Millibar                  = SCALED<double,dim::Pressure,1e2>;
        using Milligram                 = SCALED<double,dim::Mass,1e-6>;
        using MilligramPerSecond        = SCALED<double,dim::MassFlow,1e-6>;
        using Millihertz                = SCALED<double,dim::Frequency,1e-3>;
        using Millijoule                = SCALED<double,dim::Energy,1e-3>;
        using Millilitre                = SCALED<double,dim::Volume, 1e-6>;
        using Millimeter                = SCALED<double,dim::Length,0.001>;
        using Millimeter²               = SCALED<double,dim::Area,1e-6>;
        using MillimeterPerSecond       = SCALED<double,dim::Speed,1e-3>;
        using MillimeterPerSecond²      = SCALED<double,dim::Acceleration,1e-56>;
        using MillimeterPerHour         = SCALED<double,dim::Speed,1e-3/3600.>;
        using Millinewton               = SCALED<double,dim::Force,1e-3>;
        using Millipascal               = SCALED<double,dim::Pressure,1e-3>;
        using Milliradian               = SCALED<double,dim::Angle,1e-3>;
        using MilliradianPerSecond      = SCALED<double,dim::AngularVelocity,1e-3>;
        using MilliradianPerSecond²     = SCALED<double,dim::AngularAcceleration,1e-3>;
        using Milliwatt                 = SCALED<double,dim::Power,1e-3>;
        using Minute                    = SCALED<double,dim::Time, 60.0>;   
        using Mole                      = MKS<double,dim::Matter>;
        using Nanoampere                = SCALED<double,dim::Current,1e-9>;
        using Nanogram                  = SCALED<double,dim::Mass,1e-12>;
        using NanogramPerSecond         = SCALED<double,dim::MassFlow,1e-12>;
        using Nanojoule                 = SCALED<double,dim::Energy,1e-9>;
        using Nanometer                 = SCALED<double,dim::Length,1e-9>;
        using NanometerPerSecond        = SCALED<double,dim::Speed,1e-9>;
        using NanometerPerSecond²       = SCALED<double,dim::Acceleration,1e-9>;
        using Nanowatt                  = SCALED<double,dim::Power,1e-9>;
        using NauticalMile              = SCALED<double,dim::Length,1852.>;
        using Newton                    = MKS<double,dim::Force>;
        using NewtonPerSecond           = MKS<double,dim::ForceRate>;
        using NewtonSecond              = MKS<double,dim::Impulse>;
        using Pascal                    = MKS<double,dim::Pressure>;
        using Percent                   = SCALED<double,dim::None, 0.01>;
        using PerRadian                 = MKS<double,dim::PerAngle>;
        using PerMeter                  = MKS<double,dim::PerLength>;
        using PerMole                   = MKS<double,dim::PerMatter>;
        using PoundMass                 = SCALED<double,dim::Mass,0.45359237>;
        using PoundForce                = SCALED<double,dim::Force,4.4482216152605>;
        using PoundForcePerSecond       = SCALED<double,dim::ForceRate,4.4482216152605>;
        using PoundPerInch²             = SCALED<double,dim::Pressure,4.4482216152605/(0.0254*0.0254)>;
        using PoundPerFoot²             = SCALED<double,dim::Pressure,4.4482216152605/(0.3048*0.3048)>;
        using Radian                    = MKS<double,dim::Angle>;
        using Radian²                   = MKS<double,dim::SolidAngle>;
        using RadianF                   = MKS<float,dim::Angle>;
        using RadianPerSecond           = MKS<double,dim::AngularVelocity>;
        using RadianPerSecond²          = MKS<double,dim::AngularAcceleration>;
        using Rankine                   = SCALED<double,dim::Temperature,5./9.>;
        using RotationsPerMinute        = SCALED<double,dim::AngularVelocity,pi/30.>;
        using Scalar                    = MKS<double,dim::None>;
        using Second                    = MKS<double,dim::Time>;
        using Second²                   = MKS<double,dim::TimeSquare>;
        using ShortTon                  = SCALED<double,dim::Mass,907.18474>;
        using Slug                      = SCALED<double,dim::Mass,1.459390293720636482939632545931759e1>;
        using Steradian                 = MKS<double,dim::SolidAngle>;
        using TeraElectronVolt          = SCALED<double,dim::Energy,1.602176634-7>;
        using Terahertz                 = SCALED<double,dim::Frequency,1e12>;
        using Terawatt                  = SCALED<double,dim::Power,1e12>;
        using Tonne                     = SCALED<double,dim::Mass,1000.00>;
        using Watt                      = MKS<double,dim::Power>;
        using WattPerMeter³             = MKS<double,dim::PowerDensity>;
        using Week                      = SCALED<double,dim::Time, 604800.0>;
        using Yard                      = SCALED<double,dim::Length, 0.9144>;
        
        using CentimeterPerSecond3D     = Vector3<CentimeterPerSecond>;
        using Foot3D                    = Vector3<Foot>;
        using Foot3F                    = Vector3<FootF>;
        using KilometerPerSecond3D      = Vector3<KilometerPerSecond>;
        using Meter3D                   = Vector3<Meter>;
        using Meter3F                   = Vector3<MeterF>;
        using MeterPerSecond3D          = Vector3<MeterPerSecond>;
        using MeterPerSecond²3D         = Vector3<MeterPerSecond²>;
        using MicrometerPerSecond3D     = Vector3<MicrometerPerSecond>;
        using MillimeterPerSecond3D     = Vector3<MillimeterPerSecond>;
        using NanometerPerSecond3D      = Vector3<NanometerPerSecond>;
        using Newton3D                  = Vector3<Newton>;
        using Pascal33D                 = Tensor33<Pascal>;
        using Radian3D                  = Vector3<Radian>;
        using RadianPerSecond3D         = Vector3<RadianPerSecond>;
        using RadianPerSecond²3D        = Vector3<RadianPerSecond²>;
    }

    //  ------------------------------------------------
    //  FORWARD DECLARE VECTORS & TENSOR (RELATED)
    
    using Quaternion3D              = Quaternion3<double>;
    using Quaternion3F              = Quaternion3<float>;

    using Tensor11D                 = Tensor11<double>;
    using Tensor11F                 = Tensor11<float>;
    using Tensor11I                 = Tensor11<int>;
    using Tensor11U                 = Tensor11<unsigned>;

    using Tensor12D                 = Tensor12<double>;
    using Tensor12F                 = Tensor12<float>;
    using Tensor12I                 = Tensor12<int>;
    using Tensor12U                 = Tensor12<unsigned>;

    using Tensor13D                 = Tensor13<double>;
    using Tensor13F                 = Tensor13<float>;
    using Tensor13I                 = Tensor13<int>;
    using Tensor13U                 = Tensor13<unsigned>;

    using Tensor14D                 = Tensor14<double>;
    using Tensor14F                 = Tensor14<float>;
    using Tensor14I                 = Tensor14<int>;
    using Tensor14U                 = Tensor14<unsigned>;

    using Tensor21D                 = Tensor21<double>;
    using Tensor21F                 = Tensor21<float>;
    using Tensor21I                 = Tensor21<int>;
    using Tensor21U                 = Tensor21<unsigned>;

    using Tensor22D                 = Tensor22<double>;
    using Tensor22F                 = Tensor22<float>;
    using Tensor22I                 = Tensor22<int>;
    using Tensor22U                 = Tensor22<unsigned>;

    using Tensor23D                 = Tensor23<double>;
    using Tensor23F                 = Tensor23<float>;
    using Tensor23I                 = Tensor23<int>;
    using Tensor23U                 = Tensor23<unsigned>;

    using Tensor24D                 = Tensor24<double>;
    using Tensor24F                 = Tensor24<float>;
    using Tensor24I                 = Tensor24<int>;
    using Tensor24U                 = Tensor24<unsigned>;

    using Tensor31D                 = Tensor31<double>;
    using Tensor31F                 = Tensor31<float>;
    using Tensor31I                 = Tensor31<int>;
    using Tensor31U                 = Tensor31<unsigned>;

    using Tensor32D                 = Tensor32<double>;
    using Tensor32F                 = Tensor32<float>;
    using Tensor32I                 = Tensor32<int>;
    using Tensor32U                 = Tensor32<unsigned>;

    using Tensor33D                 = Tensor33<double>;
    using Tensor33F                 = Tensor33<float>;
    using Tensor33I                 = Tensor33<int>;
    using Tensor33U                 = Tensor33<unsigned>;

    using Tensor34D                 = Tensor34<double>;
    using Tensor34F                 = Tensor34<float>;
    using Tensor34I                 = Tensor34<int>;
    using Tensor34U                 = Tensor34<unsigned>;

    using Tensor41D                 = Tensor41<double>;
    using Tensor41F                 = Tensor41<float>;
    using Tensor41I                 = Tensor41<int>;
    using Tensor41U                 = Tensor41<unsigned>;

    using Tensor42D                 = Tensor42<double>;
    using Tensor42F                 = Tensor42<float>;
    using Tensor42I                 = Tensor42<int>;
    using Tensor42U                 = Tensor42<unsigned>;

    using Tensor43D                 = Tensor43<double>;
    using Tensor43F                 = Tensor43<float>;
    using Tensor43I                 = Tensor43<int>;
    using Tensor43U                 = Tensor43<unsigned>;

    using Tensor44D                 = Tensor44<double>;
    using Tensor44F                 = Tensor44<float>;
    using Tensor44I                 = Tensor44<int>;
    using Tensor44U                 = Tensor44<unsigned>;    

    using Vector1D                  = Vector1<double>;
    using Vector1F                  = Vector1<float>;
    using Vector1I                  = Vector1<int>;
    using Vector1U                  = Vector1<unsigned>;

    using Vector2D                  = Vector2<double>;
    using Vector2F                  = Vector2<float>;
    using Vector2I                  = Vector2<int>;
    using Vector2U                  = Vector2<unsigned>;

    using Vector3D                  = Vector3<double>;
    using Vector3F                  = Vector3<float>;
    using Vector3I                  = Vector3<int>;
    using Vector3U                  = Vector3<unsigned>;

    using Vector4D                  = Vector4<double>;
    using Vector4F                  = Vector4<float>;
    using Vector4I                  = Vector4<int>;
    using Vector4U                  = Vector4<unsigned>;

    //  ------------------------------------------------
    //  FORWARD DECLARE SHAPES


    using AxBox1D                   = AxBox1<double>;
    using AxBox1F                   = AxBox1<float>;
    using AxBox1I                   = AxBox1<int>;
    using AxBox1U                   = AxBox1<unsigned>;

    using AxBox2D                   = AxBox2<double>;
    using AxBox2F                   = AxBox2<float>;
    using AxBox2I                   = AxBox2<int>;
    using AxBox2U                   = AxBox2<unsigned>;

    using AxBox3D                   = AxBox3<double>;
    using AxBox3F                   = AxBox3<float>;
    using AxBox3I                   = AxBox3<int>;
    using AxBox3U                   = AxBox3<unsigned>;

    using AxBox4D                   = AxBox4<double>;
    using AxBox4F                   = AxBox4<float>;
    using AxBox4I                   = AxBox4<int>;
    using AxBox4U                   = AxBox4<unsigned>;

    using Circle2D                  = Circle2<double>;
    using Circle2F                  = Circle2<float>;
    using Circle2I                  = Circle2<int>;
    using Circle2U                  = Circle2<unsigned>;

    using Coord2D                   = Coord2<double>;
    using Coord2F                   = Coord2<float>;
    using Coord2I                   = Coord2<int>;
    using Coord2U                   = Coord2<unsigned>;

    using FractionI                 = Fraction<int>;
    //using Fraction8      = Fraction<int8_t>;
    //using Fraction16     = Fraction<int16_t>;
    //using Fraction32     = Fraction<int32_t>;
    //using Fraction64     = Fraction<int64_t>;
    
    using Normal2D                  = Normal2<double>;
    using Normal2F                  = Normal2<float>;
    
    using Normal3D                  = Normal3<double>;
    using Normal3F                  = Normal3<float>;


    using Plane3D                   = Plane3<double>;
    using Plane3F                   = Plane3<float>;

    
    using Ray2D                     = Ray2<double>;
    using Ray2F                     = Ray2<float>;

    using Ray3D                     = Ray3<double>;
    using Ray3F                     = Ray3<float>;

    using Ray4D                     = Ray4<double>;
    using Ray4F                     = Ray4<float>;
    
    using Rectangle2D               = Rectangle2<double>;
    using Rectangle2F               = Rectangle2<float>;
    using Rectangle2I               = Rectangle2<int>;
    using Rectangle2U               = Rectangle2<unsigned>;

    using RGB3D                     = RGB<double>;
    using RGB3F                     = RGB<float>;
    using RGB3I                     = RGB<int>;
    using RGB3U                     = RGB<unsigned>;
    using RGB3U8                    = RGB<uint8_t>;
    using RGB3U16                   = RGB<uint16_t>;

    using RGBA4D                    = RGBA<double>;
    using RGBA4F                    = RGBA<float>;
    using RGBA4U8                   = RGBA<uint8_t>;
    using RGBA4U16                  = RGBA<uint16_t>;
    
    using Segment1D                 = Segment1<double>;
    using Segment1F                 = Segment1<float>;
    using Segment1I                 = Segment1<int>;
    using Segment1U                 = Segment1<unsigned>;

    using Segment2D                 = Segment2<double>;
    using Segment2F                 = Segment2<float>;
    using Segment2I                 = Segment2<int>;
    using Segment2U                 = Segment2<unsigned>;

    using Segment3D                 = Segment3<double>;
    using Segment3F                 = Segment3<float>;
    using Segment3I                 = Segment3<int>;
    using Segment3U                 = Segment3<unsigned>;

    using Segment4D                 = Segment4<double>;
    using Segment4F                 = Segment4<float>;
    using Segment4I                 = Segment4<int>;
    using Segment4U                 = Segment4<unsigned>;
    
    using Size2D                    = Size2<double>;
    using Size2F                    = Size2<float>;
    using Size2I                    = Size2<int>;
    using Size2I8                   = Size2<int8_t>;
    using Size2I16                  = Size2<int16_t>;
    using Size2I32                  = Size2<int32_t>;
    using Size2I64                  = Size2<int64_t>;
    using Size2U                    = Size2<unsigned>;
    using Size2U8                   = Size2<int8_t>;
    using Size2U16                  = Size2<int16_t>;
    using Size2U32                  = Size2<int32_t>;
    using Size2U64                  = Size2<int64_t>;
    using Size2Z                    = Size2<size_t>;

    using Sphere3D                  = Sphere3<double>;
    using Sphere3F                  = Sphere3<float>;
    using Sphere3I                  = Sphere3<int>;
    using Sphere3U                  = Sphere3<unsigned>;
    
    using Sphere4D                  = Sphere4<double>;
    using Sphere4F                  = Sphere4<float>;
    using Sphere4I                  = Sphere4<int>;
    using Sphere4U                  = Sphere4<unsigned>;
    
    using Tetrahedron3D             = Tetrahedron3<double>;
    using Tetrahedron3F             = Tetrahedron3<float>;
    using Tetrahedron3I             = Tetrahedron3<int>;
    using Tetrahedron3U             = Tetrahedron3<unsigned>;

    using Triangle2D                = Triangle2<double>;
    using Triangle2F                = Triangle2<float>;
    using Triangle2I                = Triangle2<int>;
    using Triangle2U                = Triangle2<unsigned>;

    using Triangle3D                = Triangle3<double>;
    using Triangle3F                = Triangle3<float>;
    using Triangle3I                = Triangle3<int>;
    using Triangle3U                = Triangle3<unsigned>;

    using Triangle4D                = Triangle4<double>;
    using Triangle4F                = Triangle4<float>;
    using Triangle4I                = Triangle4<int>;
    using Triangle4U                = Triangle4<unsigned>;

    //  ------------------------------------------------
    //  Miscellaneous

    //! Call this if math isn't getting startup-initialized 
    void        initialize_math();

    template <typename T> using square_t = decltype(T()*T());
    template <typename T> using cube_t   = decltype(T()*T()*T());
    template <typename T, typename U>  using product_t     = decltype(T()*U());
    template <typename T, typename U>  using quotient_t    = decltype(T()/U());
    template <typename T> using inverse_t = decltype(T()/square_t<T>());
    
    using std::max;
    using std::min;
    using std::abs;
    
    namespace trait {
        template <typename T, typename U>  static constexpr const bool self_mul_v = std::is_same_v<T,product_t<T,U>>;
        template <typename T, typename U>  static constexpr const bool self_div_v = std::is_same_v<T,quotient_t<T,U>>;
        
        template <typename T>   static constexpr const bool is_basic_v = std::is_floating_point_v<T>;
        
    }
}

