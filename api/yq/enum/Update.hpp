////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/Binder.hpp>
#include <yq/type/Enum.hpp>
#include <yq/type/Flag.hpp>

namespace yq {

    YQ_ENUM(Update, ,
        Interior,   //<! Update any object-local data (no linkage)
        Exterior    //<! Update the object-local data (with linkage)
    )

    using UpdateFlags = Flag<Update>;

}

YQ_TYPE_DECLARE(yq::Update)
YQ_TYPE_DECLARE(yq::UpdateFlags)
