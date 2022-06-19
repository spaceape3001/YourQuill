////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Units.hpp"

namespace yq {

    // Will add more as needed....

    /*! \brief Arc-cosine
    */
    template <typename T>
    requires std::is_floating_point_v<T>
    MKS<T,dim::Angle>  acos(T r)
    {
        return { std::acos(r) };
    }

    template <typename T>
    requires std::is_floating_point_v<T>
    MKS<T,dim::Angle>  acos(MKS<T,dim::None> r)
    {
        return { std::acos(r.value) };
    }

    /*! \brief Arc-sine
    */
    template <typename T>
    requires std::is_floating_point_v<T>
    MKS<T,dim::Angle>  asin(T r)
    {
        return { std::asin(r) };
    }

    /*! \brief Arc-sine
    */
    template <typename T>
    requires std::is_floating_point_v<T>
    MKS<T,dim::Angle>  asin(MKS<T,dim::None> r)
    {
        return { std::asin(r.value) };
    }

    /*! \brief Arc-tangent
    */
    template <typename T>
    requires std::is_floating_point_v<T>
    MKS<T,dim::Angle>  atan(T r)
    {
        return { std::atan(r) };
    }

    /*! \brief Arc-tangent
    */
    template <typename T>
    requires std::is_floating_point_v<T> 
    MKS<T,dim::Angle>    atan(T y, T x)
    {
        return { std::atan2(y,x) };
    }
    
    /*! \brief Arc-tangent
    */
    template <typename T, typename DIM>
    requires std::is_floating_point_v<T> 
    MKS<T,dim::Angle>    atan(MKS<T,DIM> y, MKS<T,DIM> x)
    {
        return { std::atan2(y.value,x.avlue) };
    }

    /*! \brief Cosine
    */
    template <typename T>
    requires std::is_floating_point_v<T>
    T  cos(MKS<T,dim::Angle> r)
    {
        return std::cos(r.value);
    }

    /*! \brief Cotangent
    */
    template <typename T>
    requires std::is_floating_point_v<T>
    T  cot(MKS<T,dim::Angle> r)
    {
        //  Doing it this way because tangent is infinity at 90, 270, etc
        return std::cos(r.value) / std::sin(r.value);
    }

    /*! \brief Cosecant
    */
    template <typename T>
    requires std::is_floating_point_v<T>
    T  csc(MKS<T,dim::Angle> r)
    {
        return one_v<T> / std::sin(r.value);
    }
    

    /*! \brief Secant
    */
    template <typename T>
    requires std::is_floating_point_v<T>
    T  sec(MKS<T,dim::Angle> r)
    {
        return one_v<T> / std::cos(r.value);
    }

    /*! \brief Sine
    */
    template <typename T>
    requires std::is_floating_point_v<T>
    T  sin(MKS<T,dim::Angle> r)
    {
        return std::sin(r.value);
    }

    /*! \brief Tangent
    */
    template <typename T>
    requires std::is_floating_point_v<T>
    T  tan(MKS<T,dim::Angle> r)
    {
        return std::tan(r.value);
    }
    
}
