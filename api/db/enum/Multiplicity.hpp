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
        \enum Multiplicity
        \brief Multiplicity of something
        
        \var Restriction::Default
        \brief  Default multiplicity

        This value is meant to be "inherited" from others.
        \note Default of the default is all    

        \var Multilicity::Once 
        \brief Occurs only once
        
            When selected, this property may only appear once, and only once.
            (ie, date of birth).
        
        \var Multiplicity::Many
        \brief Multiple values
            
            When selected, this property can have multiple values.
        
        \var Multiplicity::Measurement
        
            When selected, this property will be time varying, the values
            are considered measurements.
        
        \var Multiplicity::All
            
            When selected, this property will can have multiple values at 
            any moment.  A value w/o date or time restraints will be valid
            throughout time.  
    */
    YQ_ENUM(Multiplicity, , Default, Once, Many, Serial, All )

    MV_DECLARE(Multiplicity)
}
