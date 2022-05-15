////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <yq/meta/InfoBinder.hpp>
#include <yq/type/Enum.hpp>

namespace yq {

    YQ_ENUM(Format, , 
        Markdown, 
        ASCII, 
        Plain, 
        Html, 
        Novel 
    )

}

YQ_TYPE_DECLARE(yq::Format)