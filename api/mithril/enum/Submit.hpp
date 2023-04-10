////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <meta/InfoBinder.hpp>
#include <basic/Enum.hpp>

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
