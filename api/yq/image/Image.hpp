////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <initializer_list>
#include <compare>

namespace yq {

    /*! \brief Represents an image in the database
    */
    struct Image {
        static constexpr const int kSmallWidth     =  24;
        static constexpr const int kSmallHeight    =  32;
        static constexpr const int kMediumWidth    =  64;
        static constexpr const int kMediumHeight   =  48;
        static constexpr const int kLargeWidth     = 128;
        static constexpr const int kLargeHeight    =  96;
        
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
}
