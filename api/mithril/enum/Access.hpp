////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq-toolbox/meta/InfoBinder.hpp>
#include <yq-toolbox/basic/Enum.hpp>

namespace yq {
    namespace mithril {

        /*! \brief Access amount 
        */
        YQ_ENUM(Access, ,
            Default,
            NoAccess,
            ReadOnly,
            ReadWrite,
                //  if set, prefer to write in this location *first* on specified
                //  information
            WriteFirst
        )
    }
}

YQ_TYPE_DECLARE(yq::mithril::Access)
