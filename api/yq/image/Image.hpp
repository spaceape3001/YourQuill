////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <compare>
#include <cstdint>
#include <initializer_list>
#include <yq/enum/SizeDesc.hpp>
#include <yq/shape/Size2.hpp>

namespace yq {

    /*! \brief Represents an image in the database
    */
    struct Image {
        static constexpr const Size2<unsigned>    kSmall  = { 32, 24 };
        static constexpr const Size2<unsigned>    kMedium = { 64, 48 };
        static constexpr const Size2<unsigned>    kLarge  = { 128, 96 };
        
        static constexpr const std::initializer_list<const char*>   kSupportedExtensions = {
            "svg", "png", "gif", "jpg", "bmp", "tif", "tiff" 
        };
        static constexpr const std::initializer_list<const char*>   kSupportedExtensionWildcards = {
            "*.svg", "*.png", "*.gif", "*.jpg", "*.bmp", "*.tif", "*.tiff" 
        };

        uint64_t  id = 0ULL;
        constexpr auto    operator<=>(const Image& rhs) const = default;
        operator bool() const { return id != 0ULL; }
    };

    struct Thumbnail {
        Image       img;
        SizeDesc    size;
        std::string alt;
        
        bool operator==(const Thumbnail&) const = default;
        operator bool() const { return img; }
    };

}
