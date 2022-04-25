////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/InfoBinder.hpp>
#include <yq/type/Enum.hpp>

namespace yq {

    /*! \brief An area of policy

    */
    YQ_ENUM(DataRole, , 
        //!  For all non-specific stuff
        Other,
        
        //! Wiki elements (the user files)
        DB,

        //! CSS style
        Style,  
        
        //! Calendar Configuration
        CfgCal,
        
        //! DB Config stuff (classes, categories, tags, etc)
        Config,
        
        //! Users
        Users,
        
        //! Images
        Image
    )

}

YQ_TYPE_DECLARE(yq::DataRole)
