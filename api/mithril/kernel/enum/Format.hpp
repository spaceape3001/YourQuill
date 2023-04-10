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
