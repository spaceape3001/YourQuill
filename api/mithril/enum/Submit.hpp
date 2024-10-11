////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/InfoBinder.hpp>
#include <yq/core/Enum.hpp>

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
