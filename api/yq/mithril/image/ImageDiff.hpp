////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/net/ContentType.hpp>
#include <yq/shape/Size2.hpp>

#include <yq/mithril/enum/Change.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/util/Changed.hpp>

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
