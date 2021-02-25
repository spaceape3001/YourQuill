////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "util/Enum.hpp"
#include "meta/MetaValue.hpp"


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

