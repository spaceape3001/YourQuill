////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/UniqueID.hpp>
#include <engine/preamble.hpp>
#include <engine/ViewerCreateInfo.hpp>
#include <engine/vulqan/VqAllocator.hpp>
#include <engine/vulqan/VqCommandBuffers.hpp>
#include <engine/vulqan/VqCommandPool.hpp>
#include <engine/vulqan/VqDescriptorPool.hpp>
#include <engine/vulqan/VqDevice.hpp>
#include <engine/vulqan/VqFence.hpp>
#include <engine/vulqan/VqFrameBuffers.hpp>
#include <engine/vulqan/VqImageViews.hpp>
#include <engine/vulqan/VqMonitor.hpp>
#include <engine/vulqan/VqPipeline.hpp>
#include <engine/vulqan/VqRenderPass.hpp>
#include <engine/vulqan/VqSemaphore.hpp>
#include <engine/vulqan/VqSurface.hpp>
#include <engine/vulqan/VqSwapchain.hpp>
#include <engine/vulqan/VqQueues.hpp>
#include <math/preamble.hpp>
#include <atomic>
#include <thread>

struct GLFWwindow;


namespace yq {
    namespace engine {
        class Viewer;
        struct ViewerCreateInfo;
        struct VqObject;
        
        //! This is what needs to change with every resize!
        struct VqDynamic {
            std::vector<VkImage>        images;
            VqSwapchain                 swapchain;
            VqCommandBuffers            commandBuffers;
            uint32_t                    imageCount          = 0;
            VqImageViews                imageViews;
            VqFrameBuffers              frameBuffers;
        };
        
        //  TODO.... 
        struct ViShader {
        };

        struct ViBuffer : trait::not_copyable, trait::not_moveable {
            std::unique_ptr<VqBuffer>   vq;
            uint64_t                    rev = 0;
            
            ViBuffer();
            ~ViBuffer();
        };

        struct ViBufferMap : trait::not_copyable, trait::not_moveable {
            std::map<uint64_t, ViBuffer*>    buffers;
            std::pair<ViBuffer*,bool>    buffer(uint64_t i)
            {
                auto j = buffers.find(i);
                if(j!=buffers.end())
                    return {j->second,false};
                ViBuffer*p  = new ViBuffer;
                buffers[i] = p;
                return {p,true};
            }
            
            ViBufferMap();
            ~ViBufferMap();
        };

        struct ViPipeline : public ViBufferMap {
            std::unique_ptr<VqPipeline> vq;
            
            ViPipeline();
            ~ViPipeline();
        };
        
        struct ViObject : public ViBufferMap {
        
            ViObject();
            ~ViObject();
        };
        

        /*! \brief Visualizer is the private data for the viewer
                
            This structure is the raw vulkan/GLFW/etc
        */
        struct Visualizer : public UniqueID {
            ViewerCreateInfo                    create_info;
            Viewer*                             user        = nullptr;
            VkInstance                          instance    = nullptr;
            VkPhysicalDevice                    physical    = nullptr;
            VkPhysicalDeviceProperties          device_info;
            VkPhysicalDeviceMemoryProperties    memory_info;
            GLFWwindow*                         window   = nullptr;
            
            VqSurface           surface;
            VqDevice            device;
            VqAllocator         allocator;
            VqCommandPool       commandPool;
            VkPresentModeKHR    presentMode                 = {};
            VqRenderPass        renderPass;
            VqSemaphore         imageAvailableSemaphore;
            VqSemaphore         renderFinishedSemaphore;
            VqFence             inFlightFence;
            VqDescriptorPool    descriptorPool;

            VkClearValue        clear;
            std::atomic<bool>   rebuildSwap                 = { false };

            VqDynamic           dynamic;
            uint64_t            tick                        = 0;
            double              draw_time                   = 0;
            std::thread         builder;
            uint64_t            pad[8];
            std::atomic<bool>   terminating                 = false;
            
            std::map<uint64_t, ViPipeline*>     pipelines;
            std::map<uint64_t, ViObject*>       objects;
            
            std::pair<ViPipeline*,bool>     pipeline(uint64_t i);
            std::pair<ViObject*,bool>       object(uint64_t i);
            
            bool    init(VqDynamic&, VkSwapchainKHR old=nullptr);
            void    kill(VqDynamic&);
            void    set_clear(const RGBA4F&);

            void    run();
            
            
            Visualizer(const ViewerCreateInfo&, Viewer*);
            ~Visualizer();
            void                _ctor();
            void                _dtor();

            std::string_view    device_name() const;
            uint32_t            max_memory_allocation_count() const noexcept;
            uint32_t            max_push_constants_size() const noexcept;
            uint32_t            max_viewports() const noexcept;
            
            static void         callback_resize(GLFWwindow*, int, int);
            
        };
    }
}
