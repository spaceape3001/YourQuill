////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/UniqueID.hpp>
#include <engine/preamble.hpp>
#include <engine/ViewerCreateInfo.hpp>
#include <engine/vulqan/VqCommandBuffers.hpp>
#include <engine/vulqan/VqCommandPool.hpp>
#include <engine/vulqan/VqFence.hpp>
#include <engine/vulqan/VqFrameBuffers.hpp>
#include <engine/vulqan/VqImageViews.hpp>
#include <engine/vulqan/VqMonitor.hpp>
#include <engine/vulqan/VqPipeline.hpp>
#include <engine/vulqan/VqRenderPass.hpp>
#include <engine/vulqan/VqSemaphore.hpp>
#include <engine/vulqan/VqSwapchain.hpp>
#include <math/preamble.hpp>

#include <vk_mem_alloc.h>
#include <imgui.h>

#include <atomic>
#include <set>
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
        
        struct ViQueues {
            std::vector<VkQueue>    queues;
            uint32_t                family   = UINT32_MAX;
            
            void    set(VkDevice, uint32_t cnt);
            ~ViQueues();
            VkQueue operator[](uint32_t i) const;
        };
        

        /*! \brief Visualizer is the private data for the viewer
                
            This structure is the raw vulkan/GLFW/etc
            
            \note Eventually this will merge into viewer itself
        */
        struct Visualizer : public UniqueID {
            Viewer*                             m_viewer            = nullptr;
            VkInstance                          m_instance          = nullptr;
            VkPhysicalDevice                    m_physical          = nullptr;
            VkPhysicalDeviceProperties          m_deviceInfo;
            VkPhysicalDeviceMemoryProperties    m_memoryInfo;
            GLFWwindow*                         m_window            = nullptr;
            VkSurfaceKHR                        m_surface           = nullptr;
            std::set<VkPresentModeKHR>          m_presentModes;
            std::vector<VkSurfaceFormatKHR>     m_surfaceFormats;
            VkFormat                            m_surfaceFormat;
            VkColorSpaceKHR                     m_surfaceColorSpace;
            std::vector<const char*>            m_extensions;
            VkDevice                            m_device            = nullptr;
            ViQueues                            m_graphic, m_present, m_compute, m_videoEncode, m_videoDecode;
            VmaAllocator                        m_allocator         = nullptr;
            VkDescriptorPool                    m_descriptorPool    = nullptr;
            
            VqCommandPool       commandPool;
            VkPresentModeKHR    presentMode                 = {};
            VqRenderPass        renderPass;
            VqSemaphore         imageAvailableSemaphore;
            VqSemaphore         renderFinishedSemaphore;
            VqFence             inFlightFence;

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
            void                _ctor(const ViewerCreateInfo&);
            void                _dtor();

            std::string_view            device_name() const;
            bool                        does_surface_support(VkPresentModeKHR) const;
            bool                        does_surface_support(VkFormat) const;


            uint32_t                    max_memory_allocation_count() const noexcept;
            uint32_t                    max_push_constants_size() const noexcept;
            uint32_t                    max_viewports() const noexcept;
            VkSurfaceCapabilitiesKHR    surface_capabilities() const;

            VkColorSpaceKHR             surface_color_space(VkFormat) const;
            VkFormat                    surface_format() const { return m_surfaceFormat; }


            static void                 callback_resize(GLFWwindow*, int, int);
            
        };
    }
}
