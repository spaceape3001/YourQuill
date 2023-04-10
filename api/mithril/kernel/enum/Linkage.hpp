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

        YQ_ENUM(Linkage, ,
            None,
            Any,
            Indirect,
            Direct
        )

    }
}

YQ_TYPE_DECLARE(yq::mithril::Linkage)
