////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <compare>
#include <cstdint>
#include <initializer_list>
#include <mithril/enum/SizeDesc.hpp>
#include <yq/shape/Size2.hpp>
#include <mithril/fragment/Fragment.hpp>

namespace yq::mithril {

    /*! \brief Represents an image in the database
    */
    struct Image {
        static constexpr const IdTypeId ID      = 19;
        static constexpr const IdTypes  PARENTS  = Fragment::ID;
        static constexpr const Size2U   kSmall  = { 32, 24 };
        static constexpr const Size2U   kMedium = { 64, 48 };
        static constexpr const Size2U   kLarge  = { 128, 96 };
        
        static constexpr const std::initializer_list<const char*>   kSupportedExtensions = {
            "svg", "png", "gif", "jpg", "bmp", "tif", "tiff" 
        };
        static constexpr const std::initializer_list<const char*>   kSupportedExtensionWildcards = {
            "*.svg", "*.png", "*.gif", "*.jpg", "*.bmp", "*.tif", "*.tiff" 
        };

        struct Diff;
        struct Info;
        using Notify = Notifier<const Diff&>;

        uint64_t  id = 0ULL;
        constexpr auto    operator<=>(const Image& rhs) const noexcept = default;
        constexpr operator uint64_t () const noexcept { return id; }
    };

    struct Thumbnail {
        Image       img;
        SizeDesc    size;
        std::string alt;
        
        bool operator==(const Thumbnail&) const = default;
        operator bool() const { return img != Image{}; }
    };

}

YQ_TYPE_DECLARE(yq::mithril::Image)
YQ_TYPE_DECLARE(yq::mithril::ImageSet)
YQ_TYPE_DECLARE(yq::mithril::ImageVector)
