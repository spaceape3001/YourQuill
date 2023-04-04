////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <meta/InfoBinder.hpp>
#include <basic/Enum.hpp>

namespace yq {

    YQ_ENUM(Submit,,
        Default,
        Save,
        Create
    )
}

YQ_TYPE_DECLARE(yq::Submit)
