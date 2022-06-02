////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

/*
    YES, we're leveraging the "hello triangle" tutorial
    
    https://vulkan-tutorial.com/
*/

#include "VqWindow.hpp"
#include "VqUtils.hpp"

#include <engine/app/EngineApp.hpp>
#include <engine/app/EngineAppImpl.hpp>
#include <basic/CollectionUtils.hpp>
#include <basic/Logging.hpp>
#include <basic/meta/ObjectInfoWriter.hpp>
#include <math/shape/Size2.hpp>
#include <math/vec/Vec2.hpp>

#include <algorithm>
#include <cassert>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

YQ_OBJECT_IMPLEMENT(yq::VqWindow)

namespace yq {
    void VqWindow::poll_events()
    {
        glfwPollEvents();
    }
    
    VqWindowInfo::VqWindowInfo(std::string_view kname, ObjectInfo& base, const std::source_location& sl) :
        ObjectInfo(kname, base, sl)
    {
    }

    ////////////////////////////////////////////////////////////////////////////////

    VqWindow::VqWindow(const WindowCreateInfo&i)
    {
        _init(i);
    }
    
    VqWindow::~VqWindow()
    {
        _deinit();
    }

    void VqWindow::_deinit()
    {
        if(m_device){
            vkDeviceWaitIdle(m_device);
        }
    
        if(m_renderPass){
            vkDestroyRenderPass(m_device, m_renderPass, nullptr);
            m_renderPass    = nullptr;
        }
    
        _destroy_renderpass();
        
        m_swap.kill(this);
        
        if(m_device){
            vkDestroyDevice(m_device, nullptr);
            m_device   = nullptr;
        }

        if(m_surface){
            vkDestroySurfaceKHR(EngineApp::instance(), m_surface, nullptr);
            m_surface  = nullptr;
        }

        if(m_window){
            glfwDestroyWindow(m_window);
            m_window    = nullptr;
        }
        
        m_physical = nullptr;
    }

    void VqWindow::_destroy_renderpass()
    {
        if(m_renderPass){
            vkDestroyRenderPass(m_device, m_renderPass, nullptr);
            m_renderPass    = nullptr;
        }
    }

    
    bool VqWindow::_init(const WindowCreateInfo& i)
    {
        VkInstance     inst  = EngineApp::instance();
        if(!inst){
            yCritical() << "Vulkan has not been initialized!";
            return false;
        }
        const EngineApp*   appapp    = EngineApp::app();
        const EngineApp::Impl*  app = appapp->m.get();
        
        
        if(!_init_physical(i.device))
            return false;
        if(!_init_window(i))
            return false;
        if(!_init_surface())
            return false;
        

        /*
            Create our logical device
        */
        
        std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
    
        auto queueInfos         = vqFindQueueFamilies(m_physical, m_surface);
        if(!queueInfos.graphicsFamily.has_value()){
            yCritical() << "Unable to get a queue with graphic capability!";
            _deinit();
            return false;
        }
        if(!queueInfos.presentFamily.has_value()){
            yCritical() << "Unable to find a present queue!";
            _deinit();
            return false;
        }
    
        std::vector<VkDeviceQueueCreateInfo> qci;
        float quePri    = 1.0f;
        
        for(uint32_t i : std::set<uint32_t>({ queueInfos.graphicsFamily.value(), queueInfos.presentFamily.value()})){
            VqDeviceQueueCreateInfo info;
            info.queueFamilyIndex = i;
            info.queueCount = 1;
            info.pQueuePriorities    = &quePri;
            qci.push_back(info);
        }
    
        VkPhysicalDeviceFeatures    df{};
        VqDeviceCreateInfo          dci;
        dci.pQueueCreateInfos        = qci.data();
        dci.queueCreateInfoCount     = (uint32_t) qci.size();
        dci.pEnabledFeatures         = &df;
        
        dci.enabledLayerCount        = (uint32_t) app->layers.size();
        if(dci.enabledLayerCount)
            dci.ppEnabledLayerNames  = app->layers.data();
    
        dci.enabledExtensionCount    = (uint32_t) deviceExtensions.size();
        if(!deviceExtensions.empty())
            dci.ppEnabledExtensionNames = deviceExtensions.data();
        
        if(vkCreateDevice(m_physical, &dci, nullptr, &m_device) != VK_SUCCESS){
            yCritical() << "Unable to create logical device!";
            _deinit();
            return false;
        }
        
            /*
                Graphics queues
            */
        vkGetDeviceQueue(m_device, queueInfos.graphicsFamily.value(), 0, &m_graphicsQueue);
        vkGetDeviceQueue(m_device, queueInfos.presentFamily.value(), 0, &m_presentQueue);
        
    
        auto    pmodes              = make_set(vqGetPhysicalDeviceSurfacePresentModesKHR(m_physical, m_surface));
        
        //for(auto& sf : vqGetPhysicalDeviceSurfaceFormatsKHR(m_physical, m_surface)){
            //yInfo() << "Format available... " << to_string(sf.format) << "/" << to_string(sf.colorSpace);
        //}

    
            /*
                SWAP CHAIN
            */
    
        m_presentMode               = pmodes.contains(i.pmode) ? i.pmode : VK_PRESENT_MODE_FIFO_KHR;
            // I know this is available on my card, get smarter later.
        m_surfaceFormat.format      = VK_FORMAT_B8G8R8A8_SRGB;
        m_surfaceFormat.colorSpace  = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    
        m_swap.init(this);
            
        if(!_init_renderpass())
            return false;
            
        return true;
    }


    bool VqWindow::_init_physical(VkPhysicalDevice dev)
    {
        if(!dev)
            dev  = vqFirstDevice();
        if(!dev){
            yCritical() << "Cannot create window without any devices!";
            return false;
        }
        m_physical  = dev;
        return true;
    }

    bool    VqWindow::_init_renderpass()
    {
            //  Render pass
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = m_surfaceFormat.format;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;        
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        
        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;        

        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        
        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;

        if (vkCreateRenderPass(m_device, &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS) {
            yCritical() << "Unable to create the render pass!";
            return false;
        } else
            return true;
    }


    bool VqWindow::_init_surface()
    {
        if(glfwCreateWindowSurface(EngineApp::instance(), m_window, nullptr, &m_surface) != VK_SUCCESS){
            yCritical() << "Unable to create window surface!";
            return false;
        }
        return true;
    }
    

    bool    VqWindow::_init_window(const WindowCreateInfo&i)
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_FLOATING, i.floating ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, i.decorated ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, i.resizable ? GLFW_TRUE : GLFW_FALSE);

        m_window = glfwCreateWindow(std::max(1,i.size.width()), std::max(1,i.size.height()), i.title, i.monitor.monitor(), nullptr);
        if(!m_window){
            yError() << "Unable to create window.";
            return false;
        }
        
        glfwSetWindowUserPointer(m_window, this);
        return true;
    }
    

    ////////////////////////////////////////////////////////////////////////////////

    void VqWindow::attention()
    {
        if(m_window)
            glfwRequestWindowAttention(m_window);
    }

    void VqWindow::close()
    {
        if(m_window)
            glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    }

    void VqWindow::focus()
    {
        if(m_window)
            glfwFocusWindow(m_window);
    }

    int  VqWindow::height() const
    {
        if(!m_window)
            return 0;
    
        int ret;
        glfwGetWindowSize(m_window, nullptr, &ret);
        return ret;
    }

    void VqWindow::hide()
    {
        if(m_window)
            glfwHideWindow(m_window);
    }

    void VqWindow::iconify()
    {
        if(m_window)
            glfwIconifyWindow(m_window);
    }

    bool        VqWindow::is_decorated() const
    {
        if(!m_window)
            return false;
        return glfwGetWindowAttrib(m_window, GLFW_DECORATED) != 0;
    }
    
    bool        VqWindow::is_focused() const
    {
        if(!m_window)
            return false;
        return glfwGetWindowAttrib(m_window, GLFW_FOCUSED ) != 0;
    }
    
    bool        VqWindow::is_floating() const
    {
        if(!m_window)
            return false;
        return glfwGetWindowAttrib(m_window, GLFW_FLOATING) != 0;
    }
    
    bool        VqWindow::is_fullscreen() const
    {
        if(!m_window)
            return false;
        return glfwGetWindowMonitor(m_window) != nullptr;
    }
    
    bool        VqWindow::is_hovered() const
    {
        if(!m_window)
            return false;
        return glfwGetWindowAttrib(m_window, GLFW_HOVERED) != 0;
    }
    
    bool        VqWindow::is_iconified() const
    {
        if(!m_window)
            return false;
        return glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) != 0;
    }

    bool        VqWindow::is_maximized() const
    {
        if(!m_window)
            return false;
        return glfwGetWindowAttrib(m_window, GLFW_MAXIMIZED) != 0;
    }
    
    bool        VqWindow::is_resizable() const
    {
        if(!m_window)
            return false;
        return glfwGetWindowAttrib(m_window, GLFW_RESIZABLE) != 0;
    }
    
    bool        VqWindow::is_visible() const
    {
        if(!m_window)
            return false;
        return glfwGetWindowAttrib(m_window, GLFW_VISIBLE) != 0;
    }
    

    void        VqWindow::maximize()
    {
        if(m_window)
            glfwMaximizeWindow(m_window);
    }

    VqMonitor   VqWindow::monitor() const
    {
        if(m_window)
            return VqMonitor(glfwGetWindowMonitor(m_window));
        return VqMonitor();
    }

    Vec2I       VqWindow::position() const
    {
        if(!m_window)
            return {};
        Vec2I   ret;
        glfwGetWindowPos(m_window, &ret.x, &ret.y);
        return ret;
    }

    void        VqWindow::restore()
    {
        if(m_window)
            glfwRestoreWindow(m_window);
    }

    void        VqWindow::set_position(const Vec2I& pos)
    {
        set_position(pos.x, pos.y);
    }
    
    void        VqWindow::set_position(int x, int y)
    {
        if(m_window){
            glfwSetWindowPos(m_window, x, y);
        }
    }

    void        VqWindow::set_size(const Size2I& sz)
    {
        set_size(sz.x, sz.y);
    }

    void        VqWindow::set_size(int w, int h)
    {
        if(m_window){
            glfwSetWindowSize(m_window, std::max(1, w), std::max(1, h));
        }
    }

    void        VqWindow::set_title(const char*z)
    {
        if(m_window && z){
            glfwSetWindowTitle(m_window, z);
        }
    }

    void        VqWindow::set_title(const std::string&z)
    {
        set_title(z.c_str());
    }

    bool        VqWindow::should_close() const
    {
        if(!m_window) 
            return true;
        return glfwWindowShouldClose(m_window);
    }

    void        VqWindow::show()
    {
        if(m_window)
            glfwShowWindow(m_window);
    }

    Size2I      VqWindow::size() const
    {
        if(!m_window)
            return {};
        Size2I  ret;
        glfwGetWindowSize(m_window, &ret.x, &ret.y);
        return ret;
    }

    VkRect2D    VqWindow::swap_def_scissor() const
    {
        return m_swap.def_scissor();
    }
    
    VkViewport  VqWindow::swap_def_viewport() const
    {
        return m_swap.def_viewport();
    }

    uint32_t    VqWindow::swap_height() const
    {
        return m_swap.extents.height;
    }

    uint32_t    VqWindow::swap_width() const
    {
        return m_swap.extents.width;
    }
    
    int  VqWindow::width() const
    {
        if(!m_window)
            return 0;
    
        int ret;
        glfwGetWindowSize(m_window, &ret, nullptr);
        return ret;
    }

    ////////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////////

    VqWindow::RenderPass::RenderPass(VqWindow*)
    {
    }
    
    VqWindow::RenderPass::~RenderPass()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////

    VkRect2D    VqWindow::SwapChain::def_scissor() const
    {
        VkRect2D    ret{};
        ret.offset  = { 0, 0 };
        ret.extent  = extents;
        return ret;
    }
    
    VkViewport  VqWindow::SwapChain::def_viewport() const
    {
        VkViewport  ret{};
        ret.x = 0.0f;
        ret.y = 0.0f;
        ret.width = (float) extents.width;
        ret.height = (float) extents.height;
        ret.minDepth = 0.0f;
        ret.maxDepth = 1.0f;
        return ret;
    }
        
    bool    VqWindow::SwapChain::init(VqWindow* win)
    {
        VkSurfaceCapabilitiesKHR    capabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(win->m_physical, win->m_surface, &capabilities);
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            extents = capabilities.currentExtent;
        } else {
            int w, h;
            glfwGetFramebufferSize(win->m_window, &w, &h);
            extents = {};
            extents.width    = std::clamp((uint32_t) w, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            extents.height   = std::clamp((uint32_t) h, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
        }

        
        uint32_t    imageCount    = capabilities.minImageCount + 1;
        if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
            imageCount = capabilities.maxImageCount;
        }

        VqSwapchainCreateInfoKHR    createInfo;
        createInfo.surface          = win->m_surface;
        createInfo.minImageCount    = imageCount;
        createInfo.imageFormat      = win->m_surfaceFormat.format;
        createInfo.imageColorSpace  = win->m_surfaceFormat.colorSpace;
        createInfo.imageExtent      = extents;
        createInfo.imageArrayLayers = 1;    // we're not steroscopic
        createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        
        auto indices                = vqFindQueueFamilies(win->m_physical, win->m_surface);
        uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        if (indices.graphicsFamily != indices.presentFamily) {
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
        createInfo.presentMode      = win->m_presentMode;
        createInfo.clipped          = VK_TRUE;
        
            // TEMPORARY UNTIL WE GET THE NEW ONE
        createInfo.oldSwapchain = VK_NULL_HANDLE;
        
        if (vkCreateSwapchainKHR(win->m_device, &createInfo, nullptr, &chain) != VK_SUCCESS){
            chain   = nullptr;
            yCritical() << "Failed to create the SWAP chain!";
            return false;
        }

        vkGetSwapchainImagesKHR(win->m_device, chain, &imageCount, nullptr);
        images.resize(imageCount);
        vkGetSwapchainImagesKHR(win->m_device, chain, &imageCount, images.data());    

        imageViews.resize(images.size(), nullptr);
        for(size_t i=0;i<images.size(); ++i){
            VqImageViewCreateInfo   createInfo;
            createInfo.image        = images[i];
            createInfo.viewType     = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format       = win->m_surfaceFormat.format;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;
            if(vkCreateImageView(win->m_device, &createInfo, nullptr, &imageViews[i]) != VK_SUCCESS) {
                yCritical() << "Failed to create one of the Swap Image Viewers!";
            }
        }
        
        return true;
    }
    
    void    VqWindow::SwapChain::kill(VqWindow* win)
    {
        for(auto iv : imageViews){
            if(iv)
                vkDestroyImageView(win->m_device, iv, nullptr);
        }
        imageViews.clear();
        if(chain){
            vkDestroySwapchainKHR(win->m_device, chain, nullptr);
            chain  = nullptr;
        }
    }
    
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    
    YQ_INVOKE(
        writer<VqWindow>();
    )
}


