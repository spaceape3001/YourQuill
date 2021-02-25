////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "util/Enum.hpp"
#include "util/Flag.hpp"
#include "meta/MetaValue.hpp"

YQ_ENUM(Update, ,
    Interior,   //<! Update any object-local data (no linkage)
    Exterior    //<! Update the object-local data (with linkage)
)

using UpdateFlags = Flag<Update>;

MV_DECLARE(Update)
