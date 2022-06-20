////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Enum.hpp>
#include <basic/meta/InfoBinder.hpp>

namespace yq {
    YQ_ENUM(Tristate, ,
        INHERIT     = 1,
        NO          = 0,
        YES         = 2
    );
}

YQ_TYPE_DECLARE(yq::Tristate)

