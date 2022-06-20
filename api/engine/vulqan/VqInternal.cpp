////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "VqInternal.hpp"
#include "VqException.hpp"
#include "VqStructs.hpp"
#include "VqUtils.hpp"
#include <basic/Logging.hpp>
#include <engine/Application.hpp>
#include <engine/MainWindow.hpp>

namespace yq {
    VqInternal::VqInternal(const WindowCreateInfo& i, MainWindow *w)
    {
        user        = w;
        
        instance    = Application::vulkan();
        if(!instance)
            throw VqException("Vulkan has not been initialized!");

        VkPhysicalDevice    pdev    = i.device;
        pdev  = i.device;
        if(!pdev){
            pdev  = vqFirstDevice();
            if(!pdev)
                throw VqException("Cannot create window without any devices!");
        }
        physical                    = VqGPU(pdev);
        yNotice() << "Using (" << to_string(physical.device_type()) << "): " << physical.device_name();
        window                      = VqWindow(*this, i);

        surface                     = VqSurface(physical, window);
        
        VqDevice::Config    dci;
        dci.extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        device                      = VqDevice(surface, dci);
        allocator                   = VqAllocator(*this);
   
        presentMode                 = surface.supports(i.pmode) ? i.pmode : VK_PRESENT_MODE_FIFO_KHR;
            
        commandPool                 = VqCommandPool(device, device.graphics().family());
        renderPass                  = VqRenderPass(*this);
        imageAvailableSemaphore     = VqSemaphore(device);
        renderFinishedSemaphore     = VqSemaphore(device);
        inFlightFence               = VqFence(device);
        descriptorPool              = VqDescriptorPool(device, i.descriptors);

        if(!init(dynamic))
            throw VqException("");
        
        set_clear(i.clear);
    }

    bool VqInternal::init(VqDynamic&ds, VkSwapchainKHR old)
    {
        VqSwapchain::Config scfg;
        scfg.pmode          = presentMode;
        scfg.old            = old;
        ds.swapchain        = VqSwapchain(device, surface, scfg);
        
        ds.images           = ds.swapchain.images();
        ds.imageCount       = ds.images.size();
        ds.imageViews       = VqImageViews(device, surface, ds.images);
        ds.frameBuffers     = VqFrameBuffers(device, renderPass, ds.swapchain.extents(), ds.imageViews);
        ds.commandBuffers   = VqCommandBuffers(commandPool, 1);

        return true;            
    }

    VqInternal::~VqInternal()
    {
        if(device){
            vkDeviceWaitIdle(device);
        }
        
        kill(dynamic);
        
        descriptorPool              = {};
        imageAvailableSemaphore     = {};
        renderFinishedSemaphore     = {};
        inFlightFence               = {};
        renderPass                  = {};
        commandPool                 = {};
        
        allocator                   = {};
        device                      = {};
        
        surface                     = {};
        window                      = {};
        physical                    = {};
    }


    void VqInternal::kill(VqDynamic&ds)
    {
        ds.commandBuffers   = {};
        ds.frameBuffers     = {};
        ds.imageViews       = {};
        ds.swapchain        = {};
    }
    
    void    VqInternal::set_clear(const RGBA4F&i)
    {
        clear = VkClearValue{{{ i.red, i.green, i.blue, i.alpha }}};
    }
}
