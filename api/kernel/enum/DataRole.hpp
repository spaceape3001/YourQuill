////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <meta/InfoBinder.hpp>
#include <basic/Enum.hpp>

namespace yq {

    /*! \brief An area of policy

    */
    YQ_ENUM(DataRole, , 
        NoRole,
        
        //!  For all non-specific stuff
        Other,
        
        //! Wiki elements (the user files)
        DB,

        //! CSS style
        Style,  
        
        //! DB Config stuff (classes, categories, tags, etc)
        Config,
        
        //! Users
        Users,
        
        //! Images
        Image
    )

}

YQ_TYPE_DECLARE(yq::DataRole)
