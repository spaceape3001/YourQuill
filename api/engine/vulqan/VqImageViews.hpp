////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/trait/not_copyable.hpp>
#include <basic/trait/not_moveable.hpp>
#include <vulkan/vulkan_core.h>
#include <vector>

namespace yq {
    namespace engine {
        class VqSurface;

        class VqImageViews : trait::not_copyable {
        public:
            VqImageViews() {}
            VqImageViews(VkDevice, const VqSurface&, const std::vector<VkImage>&images);
            VqImageViews(VqImageViews&&);
            VqImageViews&          operator=(VqImageViews&&);
            ~VqImageViews();
            
            VkImageView         operator[](size_t) const;
            VkImageView         image_view(size_t) const;
            bool                empty() const { return m_imageViews.empty(); }
            size_t              size() const { return m_imageViews.size(); }
            size_t              count() const { return m_imageViews.size(); }
            
            const std::vector<VkImageView>& image_views() const { return m_imageViews; }
            operator const std::vector<VkImageView>& () const { return m_imageViews; }
            
        private:
            
            void    dtor();
            void    move(VqImageViews&&);

            VkDevice                    m_device;
            std::vector<VkImageView>    m_imageViews;
        };

    }
}
