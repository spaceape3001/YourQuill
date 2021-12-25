////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <meta/MetaValue.hpp>
#include <util/type/Enum.hpp>

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

    MV_DECLARE(Access)

}
