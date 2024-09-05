////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/image/Image.hpp>
#include <yq-toolbox/basic/ContentType.hpp>

namespace yq::mithril {
    struct Image::Info {
        //Document        doc;
        Size2U          dim;
        ContentType     type;
    };
}

