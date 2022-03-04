////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <util/meta/Binder.hpp>
#include <util/type/Enum.hpp>

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
