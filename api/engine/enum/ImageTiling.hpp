////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <basic/Enum.hpp>
#include <basic/meta/InfoBinder.hpp>

namespace yq {
    YQ_ENUM(ImageTiling, ,
        Optimal = 0,
        Linear = 1,
        DRMFormat = 1000158000
    )
}
YQ_TYPE_DECLARE(yq::ImageTiling)
