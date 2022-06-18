////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math/preamble.hpp>
#include <math/RGBA.hpp>
#include <math/shape/Size2.hpp>
#include <math/vec/Vector2.hpp>
#include <engine/vulqan/VqMonitor.hpp>
#include <vulkan/vulkan_core.h>
#include <optional>

namespace yq {
    namespace engine {
        struct WindowCreateInfo {
            VkPhysicalDevice    device   = nullptr;
        
            const char*                 title    = "(untitled)";
            Size2I                      size     = { 1920, 1080 };
            std::optional<Vector2I>     position;
                //!  Set to get full screen, windowed otherwise
            VqMonitor                   monitor;
            VkPresentModeKHR            pmode   = VK_PRESENT_MODE_FIFO_KHR;
            
            
                //!  This is the background color
            RGBA4F                      clear   = { 0., 0., 0., 1. };
            
                //!  Set to make always-on-top
            bool                        floating    = false;
            
                //!  Set to make decorated
            bool                        decorated   = true;
            
                //!  Set to make user-resizable
            bool                        resizable   = false;
            
                //!     Set to get descriptors allocated
            uint32_t                    descriptors = 0;
            
            WindowCreateInfo(){}
        };
    }
}
