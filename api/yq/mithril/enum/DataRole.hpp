////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/MetaBinder.hpp>
#include <yq/core/Enum.hpp>

namespace yq::mithril {

    /*! \brief An area of policy

    */
    enum class DataRole : uint8_t {
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
    };
}

YQ_TYPE_DECLARE(yq::mithril::DataRole)
