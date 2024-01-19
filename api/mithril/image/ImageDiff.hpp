////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/basic/ContentType.hpp>
#include <0/math/shape/Size2.hpp>

#include <mithril/enum/Change.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/util/Changed.hpp>

namespace yq::mithril {
    struct Image::Diff {
        const Image     x;
        const Change    chg;
        
        ContentType     type;
        Changed<Size2U> dim;
    };
}
