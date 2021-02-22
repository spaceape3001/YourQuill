////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "yq/util/Enum.hpp"
#include "yq/meta/MetaValue.hpp"

YQ_ENUM(Change, ,
    None,
    Added,      // fragment's been added
    Modified,   // fragment's been modified
    Removed     // fragment's been removed
)
MV_DECLARE(Change)
