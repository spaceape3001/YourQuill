////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/meta/InfoBinder.hpp>
#include <0/basic/Enum.hpp>

namespace yq {
    namespace mithril {

        YQ_ENUM(Submit,,
            Default,
            Save,
            Create
        )
    }
}

YQ_TYPE_DECLARE(yq::mithril::Submit)
