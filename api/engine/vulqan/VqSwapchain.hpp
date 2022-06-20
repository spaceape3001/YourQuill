////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/trait/not_copyable.hpp>
#include <vulkan/vulkan_core.h>
#include <vector>

namespace yq {
    class VqDevice;
    class VqSurface;

    class VqSwapchain : trait::not_copyable {
    public:
        
        struct Config {
            VkPresentModeKHR    pmode   = VK_PRESENT_MODE_FIFO_KHR;
            VkSwapchainKHR      old     = nullptr;
            
            Config(){}
        };
    
        VqSwapchain(){}
        VqSwapchain(VqDevice&, VqSurface&, const Config& cfg = Config());
        VqSwapchain(VqSwapchain&&);
        VqSwapchain& operator=(VqSwapchain&&);
        ~VqSwapchain();
        
        bool    good() const noexcept { return m_swapchain != nullptr; }
        operator bool() const noexcept { return good(); }
        operator VkSwapchainKHR() const noexcept { return m_swapchain; }
        VkSwapchainKHR  swapchain() const noexcept { return m_swapchain; }
        
        std::vector<VkImage>    images() const;
    
        VkRect2D                def_scissor() const;
        VkViewport              def_viewport() const;
        
        uint32_t                min_image_count() const { return m_minImageCount; }
        
        uint32_t                width() const { return m_extents.width; }
        uint32_t                height() const { return m_extents.height; }
        const VkExtent2D&       extents() const { return m_extents; }

    private:
        void    dtor();
        void    move(VqSwapchain&&);
        
        VkDevice        m_device    = nullptr;
        VkSwapchainKHR  m_swapchain = nullptr;
        VkExtent2D      m_extents   = { 0, 0 };
        uint32_t        m_minImageCount = 0;
    };
}
