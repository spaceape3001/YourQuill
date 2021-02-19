#pragma once

#include "yq/util/Enum.hpp"
#include "yq/meta/MetaValue.hpp"


/*! \brief An area of policy

*/
YQ_ENUM(DataRole, , 
    //!  For all non-specific stuff
    Other,
    
    //! Web elements
    DB,

    //! CSS style
    Style,  
    
    //! Calendar Configuration
    CfgCal,
    
    Tags,
    
    Classes,
    
    //! Users
    Users,
    
    //! Images
    Image
)

MV_DECLARE(DataRole)
