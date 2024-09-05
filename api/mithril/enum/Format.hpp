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

        YQ_ENUM(Format, , 
            Markdown, 
            ASCII, 
            Plain, 
            Html, 
            Novel 
        )
    }
}

YQ_TYPE_DECLARE(yq::mithril::Format)
