////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <engine/vulqan/VqAllocator.hpp>
#include <engine/vulqan/VqCommandBuffers.hpp>
#include <engine/vulqan/VqCommandPool.hpp>
#include <engine/vulqan/VqDescriptorPool.hpp>
#include <engine/vulqan/VqDevice.hpp>
#include <engine/vulqan/VqFence.hpp>
#include <engine/vulqan/VqFrameBuffers.hpp>
#include <engine/vulqan/VqGPU.hpp>
#include <engine/vulqan/VqImageViews.hpp>
#include <engine/vulqan/VqMonitor.hpp>
#include <engine/vulqan/VqRenderPass.hpp>
#include <engine/vulqan/VqSemaphore.hpp>
#include <engine/vulqan/VqSurface.hpp>
#include <engine/vulqan/VqSwapchain.hpp>
#include <engine/vulqan/VqWindow.hpp>
#include <engine/vulqan/VqQueues.hpp>
#include <math/preamble.hpp>
#include <basic/trait/not_copyable.hpp>
#include <basic/trait/not_moveable.hpp>
#include <atomic>


namespace yq {
    class MainWindow;
    struct WindowCreateInfo;
    
    //! This is what needs to change with every resize!
    struct VqDynamic {
        std::vector<VkImage>        images;
        VqSwapchain                 swapchain;
        VqCommandBuffers            commandBuffers;
        uint32_t                    imageCount          = 0;
        VqImageViews                imageViews;
        VqFrameBuffers              frameBuffers;
    };

    struct VqInternal : trait::not_copyable, trait::not_moveable {
        MainWindow*             user                        = nullptr;
        VkInstance          instance                    = nullptr;
        VqGPU               physical;
        VqWindow            window;
        VqSurface           surface;
        VqDevice            device;
        VqAllocator         allocator;
        VqCommandPool       commandPool;
        VkPresentModeKHR    presentMode               = {};
        VqRenderPass        renderPass;
        VqSemaphore         imageAvailableSemaphore;
        VqSemaphore         renderFinishedSemaphore;
        VqFence             inFlightFence;
        VqDescriptorPool    descriptorPool;

        VkClearValue        clear;
        std::atomic<bool>   rebuildSwap               = { false };

        VqDynamic           dynamic;
        
        bool    init(VqDynamic&, VkSwapchainKHR old=nullptr);
        void    kill(VqDynamic&);
        void    set_clear(const RGBA4F&);
        
        VqInternal(const WindowCreateInfo&, MainWindow*);
        ~VqInternal();
    };
}
