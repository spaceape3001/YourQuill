////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "VqBuffer.hpp"
#include <engine/Visualizer.hpp>
#include "VqException.hpp"
#include "VqPipeline.hpp"
#include "VqStructs.hpp"
#include "VqUtils.hpp"
#include <basic/Logging.hpp>
#include <engine/Application.hpp>
#include <engine/Viewer.hpp>
#include <engine/render/Pipeline.hpp>

namespace yq {
    namespace engine {

        ViBuffer::ViBuffer()
        {
        }
        
        ViBuffer::~ViBuffer()
        {
        }
        
        ////////////////////////////////////////////////////////////////////////////////

        ViBufferMap::ViBufferMap()
        {
        }
        
        ViBufferMap::~ViBufferMap()
        {
            for(auto& b : buffers){
                if(b.second)
                    delete b.second;
            }
            buffers.clear();
        }

        ////////////////////////////////////////////////////////////////////////////////

        ViObject::ViObject()
        {
        }
        
        ViObject::~ViObject()
        {
        }

        ////////////////////////////////////////////////////////////////////////////////

        ViPipeline::ViPipeline()
        {
        }
        
        ViPipeline::~ViPipeline()
        {
        }

        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////

        namespace {
            uint64_t    genInternalId()
            {
                static std::atomic<uint64_t>    last{0};
                return ++last;
            }
        }
    
        Visualizer::Visualizer(const WindowCreateInfo& i, Viewer *w) : id(genInternalId())
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

        bool Visualizer::init(VqDynamic&ds, VkSwapchainKHR old)
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

        Visualizer::~Visualizer()
        {
            terminating = true;
            builder.join();
            if(device){
                vkDeviceWaitIdle(device);
            }
            
            for(auto& i : objects){
                if(i.second)
                    delete i.second;
            }
            objects.clear();

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


        void Visualizer::kill(VqDynamic&ds)
        {
            ds.commandBuffers   = {};
            ds.frameBuffers     = {};
            ds.imageViews       = {};
            ds.swapchain        = {};
        }

        void    Visualizer::run()
        {
            using namespace std::chrono_literals;
            while(!terminating){
                std::this_thread::sleep_for(1ms);
            }
        }

        void    Visualizer::set_clear(const RGBA4F&i)
        {
            clear = VkClearValue{{{ i.red, i.green, i.blue, i.alpha }}};
        }

        std::pair<ViPipeline*,bool>    Visualizer::pipeline(uint64_t i)
        {
            auto j = pipelines.find(i);
            if(j!=pipelines.end())
                return {j->second,false};
            ViPipeline* p   = new ViPipeline;
            pipelines[i]    = p;
            return {p, true};
        }
        
        std::pair<ViObject*,bool>    Visualizer::object(uint64_t i)
        {
            auto j = objects.find(i);
            if(j!=objects.end())
                return {j->second,false};
            ViObject* p   = new ViObject;
            objects[i]    = p;
            return {p, true};
        }
        
        
    }
}
