////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/Binder.hpp>
#include <yq/type/Enum.hpp>

namespace yq {

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

YQ_TYPE_DECLARE(yq::Access)