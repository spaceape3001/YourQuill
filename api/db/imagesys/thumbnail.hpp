////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <compare>
#include <cstdint>

#include <yq/enum/SizeDesc.hpp>
#include <db/imagesys/image.hpp>

namespace yq {
    struct Thumbnail {
        Image       img;
        SizeDesc    size;
        
        bool operator==(const Thumbnail&) const = default;
        operator bool() const { return img; }
    };
}
