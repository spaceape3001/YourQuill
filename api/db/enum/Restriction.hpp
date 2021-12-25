////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <meta/MetaValue.hpp>
#include <util/type/Enum.hpp>

namespace yq {
    /*! 
        \enum Restriction
        \brief Restrictions on the values

        \var Restriction::Default
        \brief  Default restrictions
        
        This value is meant to be "inherited" from others.
        \note Default of the default is any    

        \var Restriction::Any
        \brief  No restrictions
        
        \var Restriction::Suggest
        \brief Suggest (by the user), but do not track
        
        \var Restriction::Track
        \brief Tracks the values in the fields, based on that.

        \var Restriction::Enumerate
        \brief Restricts the values to the specified, enumerated list
    */
    YQ_ENUM(Restriction, , Default, Any, Suggest, Track, Enumerate)

    MV_DECLARE(Restriction)

}
