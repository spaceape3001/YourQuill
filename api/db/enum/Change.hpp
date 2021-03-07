////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <meta/MetaValue.hpp>
#include <util/Enum.hpp>

YQ_ENUM(Change, ,
    None,
    Added,      // fragment's been added
    Modified,   // fragment's been modified
    Removed     // fragment's been removed
)
MV_DECLARE(Change)
