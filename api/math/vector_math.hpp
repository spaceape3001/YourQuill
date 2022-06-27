////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math/Vector1.hpp>
#include <math/Vector2.hpp>
#include <math/Vector3.hpp>
#include <math/Vector4.hpp>
#include <math/Units.hpp>

namespace yq {

//  --------------------------------------------------------
//  COMPOSITION

    inline Vector2D     ccw(Radian v)
    {
        return { cos(v), sin(v) };
    }
    
    inline  Vector3D    ccw(Radian az, Radian el)
    {
        double  c  = cos(el);
        return { c*cos(az), c*sin(az), sin(el) };
    }

    inline Vector2D     clockwise(Radian v)
    {
        return { sin(v), cos(v) };
    }

    inline Vector3D     clockwise(Radian az, Radian el)
    {
        double  c  = cos(el);
        return { c*sin(az), c*cos(az), sin(el) };
    }

    template <typename T>
    Vector2<T> xy( const Vector3<T>& a)
    {
        return { a.x, a.y };
    }

    template <typename T>
    Vector3<T> xy( const Vector2<T>& a, std::type_identity_t<T> z)
    {
        return { a.x, a.y, z };
    }

//  --------------------------------------------------------
//  GETTERS

//  --------------------------------------------------------
//  SETTERS


//  --------------------------------------------------------
//  BASIC FUNCTIONS

//  --------------------------------------------------------
//  POSITIVE


//  --------------------------------------------------------
//  NEGATIVE


//  --------------------------------------------------------
//  NORMALIZATION


//  --------------------------------------------------------
//  ADDITION


//  --------------------------------------------------------
//  SUBTRACTION


//  --------------------------------------------------------
//  MULTIPLICATION


//  --------------------------------------------------------
//  DIVISION

//  --------------------------------------------------------
//  DOT PRODUCT


//  --------------------------------------------------------
//  INNER PRODUCT


//  --------------------------------------------------------
//  OUTER PRODUCT


///  --------------------------------------------------------
//  OTIMES PRODUCT

//  --------------------------------------------------------
//  PROJECTIONS

//  --------------------------------------------------------
//  ADVANCED FUNCTIONS


//  --------------------------------------------------------
//  CONDITIONAL INCLUDES

}
