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
