////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "VqSwapchain.hpp"
#include "VqDevice.hpp"
#include "VqException.hpp"
#include "VqStructs.hpp"
#include "VqSurface.hpp"

#include <basic/preamble.hpp>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <limits>

namespace yq {
    namespace engine {
        VqSwapchain::VqSwapchain(VqDevice& device, VqSurface& surface, const Config& cfg) : m_device(device)
        {

            VkSurfaceCapabilitiesKHR    capabilities = surface.capabilities();
            if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
                m_extents = capabilities.currentExtent;
            } else {
                int w, h;
                glfwGetFramebufferSize(surface.window(), &w, &h);
                m_extents = {};
                m_extents.width    = std::clamp((uint32_t) w, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
                m_extents.height   = std::clamp((uint32_t) h, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
            }

            #if 0
            int w, h;
            glfwGetFramebufferSize(m_window, &w, &h);
            vqInfo << "init dymamic stuff\n"<<
            "Frame itself is [" << w << 'x' << h << "] vs\n" <<
            "Image extents is " << ds.extents << '\n' <<
            "Cur is " << capabilities.currentExtent << '\n' <<
            "Min is " << capabilities.minImageExtent << '\n' <<
            "Max is " << capabilities.maxImageExtent
            ;
            #endif

            m_minImageCount               = capabilities.minImageCount;
            if(m_minImageCount < 2)
                m_minImageCount   = 2;
            uint32_t    imageCount                  = m_minImageCount + 1;
            if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
                imageCount = capabilities.maxImageCount;
            }

            VqSwapchainCreateInfoKHR    createInfo;
            createInfo.surface          = surface;
            createInfo.minImageCount    = imageCount;
            createInfo.imageFormat      = surface.format();
            createInfo.imageColorSpace  = surface.color_space();
            createInfo.imageExtent      = m_extents;
            createInfo.imageArrayLayers = 1;    // we're not steroscopic
            createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            
            uint32_t queueFamilyIndices[] = {device.graphics().family(), device.present().family()};
            if (device.graphics().family() != device.present().family()) {
                createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                createInfo.queueFamilyIndexCount = 2;
                createInfo.pQueueFamilyIndices = queueFamilyIndices;
            } else {
                createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
                createInfo.queueFamilyIndexCount = 0; // Optional
                createInfo.pQueueFamilyIndices = nullptr; // Optional
            }        
            createInfo.preTransform     = capabilities.currentTransform;
            createInfo.compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            createInfo.presentMode      = cfg.pmode;
            createInfo.clipped          = VK_TRUE;
            
                // TEMPORARY UNTIL WE GET THE NEW ONE
            createInfo.oldSwapchain     = cfg.old;
            
            if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &m_swapchain) != VK_SUCCESS)
                throw VqException("Failed to create the SWAP chain!");
        }
        
        VqSwapchain::VqSwapchain(VqSwapchain&& mv)
        {
            move(std::move(mv));
        }
        
        VqSwapchain& VqSwapchain::operator=(VqSwapchain&&mv)
        {
            if(this != &mv){
                dtor();
                move(std::move(mv));
            }
            return *this;
        }
        
        VqSwapchain::~VqSwapchain()
        {
            dtor();
        }

        VkRect2D    VqSwapchain::def_scissor() const
        {
            VkRect2D    ret{};
            ret.offset  = { 0, 0 };
            ret.extent  = m_extents;
            return ret;
        }
        
        VkViewport  VqSwapchain::def_viewport() const
        {
            VkViewport  ret{};
            ret.x = 0.0f;
            ret.y = 0.0f;
            ret.width = (float) m_extents.width;
            ret.height = (float) m_extents.height;
            ret.minDepth = 0.0f;
            ret.maxDepth = 1.0f;
            return ret;
        }
        
        std::vector<VkImage>    VqSwapchain::images() const
        {
            uint32_t                    count = 0;
            std::vector<VkImage>        ret;
            vkGetSwapchainImagesKHR(m_device, m_swapchain, &count, nullptr);
            ret.resize(count, nullptr);
            vkGetSwapchainImagesKHR(m_device, m_swapchain, &count, ret.data());    
            return ret;
        }
        

        void    VqSwapchain::dtor()
        {
            if(m_swapchain){
                vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
                m_swapchain  = nullptr;
            }
        }
        
        void    VqSwapchain::move(VqSwapchain&&mv)
        {
            steal(m_device, mv.m_device);
            steal(m_swapchain, mv.m_swapchain);
            m_extents   = mv.m_extents;
            m_minImageCount = mv.m_minImageCount;
        }
    }
}
