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
#include <engine/Vulqan.hpp>
#include <engine/render/Pipeline.hpp>

namespace yq {
    namespace engine {
        VqInternal::VqInternal(const WindowCreateInfo& i, Vulqan *w)
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
            
            builder = std::thread([this](){
                this->run();
            });
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
            terminating = true;
            builder.join();
            if(device){
                vkDeviceWaitIdle(device);
            }
            
            for(auto& i : pipelines){
                if(i.second)
                    delete i.second;
            }
            pipelines.clear();
            
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

        void    VqInternal::run()
        {
            using namespace std::chrono_literals;
            while(!terminating){
                std::this_thread::sleep_for(1ms);
            }
        }

        const VqPipeline* VqInternal::pipeline(const Pipeline*pipe)
        {
            if(!pipe)
                return nullptr;
            auto i = pipelines.find(pipe->id());
            if(i != pipelines.end())
                return i->second;
            
            // eventually a task/queue... (later)
            VqPipeline*     p   = new VqPipeline(*this, pipe);
            pipelines[pipe->id()]   = p;
            return p;
        }
        
        void    VqInternal::set_clear(const RGBA4F&i)
        {
            clear = VkClearValue{{{ i.red, i.green, i.blue, i.alpha }}};
        }
        
    }
}
