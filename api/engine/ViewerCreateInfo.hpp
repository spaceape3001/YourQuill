////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math/preamble.hpp>
#include <math/RGBA.hpp>
#include <math/Size2.hpp>
#include <math/Vector2.hpp>
#include <engine/preamble.hpp>
#include <engine/vulqan/VqMonitor.hpp>
//#include <vulkan/vulkan_core.h>
#include <optional>

struct VkPhysicalDevice_T;

namespace yq {
    namespace engine {
        struct ViewerCreateInfo {
            VkPhysicalDevice_T*         device   = nullptr;
        
            const char*                 title    = "(untitled)";
            Size2I                      size     = { 1920, 1080 };
            std::optional<Vector2I>     position;
                //!  Set to get full screen, windowed otherwise
            VqMonitor                   monitor;
            PresentMode                 pmode   = PresentMode::Fifo;
            
            
                //!  This is the background color
            RGBA4F                      clear   = { 0., 0., 0., 1. };
            
                //!  Set to make always-on-top
            bool                        floating    = false;
            
                //!  Set to make decorated
            bool                        decorated   = true;
            
                //!  Set to make user-resizable
            bool                        resizable   = false;
            
                //!  Set to enable imgui
            bool                        imgui       = false;
            
                //!     Set to get descriptors allocated
            uint32_t                    descriptors = 0;
            
            ViewerCreateInfo(){}
        };
    }
}
