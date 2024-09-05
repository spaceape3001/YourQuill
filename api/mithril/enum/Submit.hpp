////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq-toolbox/meta/InfoBinder.hpp>
#include <yq-toolbox/basic/Enum.hpp>

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
