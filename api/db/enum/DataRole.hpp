////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <util/meta/Binder.hpp>
#include <util/type/Enum.hpp>

namespace yq {

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

}

YQ_TYPE_DECLARE(yq::DataRole)
