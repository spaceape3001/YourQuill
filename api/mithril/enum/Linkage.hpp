////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/InfoBinder.hpp>
#include <0/basic/Enum.hpp>

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
