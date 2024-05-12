////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/meta/InfoBinder.hpp>
#include <0/basic/Enum.hpp>

namespace yq {
    namespace mithril {

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
            Image,
            
            //! Doodles/Diagrams
            Doodles,
            
            //! Writings
            Novels
        )

    }
}

YQ_TYPE_DECLARE(yq::mithril::DataRole)
