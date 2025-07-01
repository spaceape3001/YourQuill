////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/MetaBinder.hpp>
#include <yq/core/Enum.hpp>

namespace yq {
    namespace mithril {
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
        YQ_ENUM(Restriction, , 
            Default, 
            Any, 
            Suggest, 
            Track, 
            Enumerate
        )

    }
}

YQ_TYPE_DECLARE(yq::mithril::Restriction)
