////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <meta/MetaValue.hpp>
#include <util/Enum.hpp>
#include <util/Flag.hpp>

YQ_ENUM(Update, ,
    Interior,   //<! Update any object-local data (no linkage)
    Exterior    //<! Update the object-local data (with linkage)
)

using UpdateFlags = Flag<Update>;

MV_DECLARE(Update)
