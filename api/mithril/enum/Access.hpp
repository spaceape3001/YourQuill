////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/InfoBinder.hpp>
#include <0/basic/Enum.hpp>

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
