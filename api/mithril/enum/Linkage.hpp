////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/MetaBinder.hpp>
#include <yq/core/Enum.hpp>

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
