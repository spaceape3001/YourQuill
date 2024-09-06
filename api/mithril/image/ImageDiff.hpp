////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq-toolbox/basic/ContentType.hpp>
#include <yq-toolbox/shape/Size2.hpp>

#include <mithril/enum/Change.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/util/Changed.hpp>

namespace yq::mithril {
    struct Image::Diff {
        const Image         x;
        const uint64_t      id;
        const Change        chg;
        
        ContentType         type;
        Changed<Size2U>     dim;

        constexpr operator bool() const noexcept 
        {
            return type || dim;
        }
    };
}
